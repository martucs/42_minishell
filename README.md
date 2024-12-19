# 42_minishell
~ Reproducing the behaviour of the bash terminal ~

In its essence, minishell has to:
> Display a prompt & wait for user input

> Read the input

> Show a result

> Display the prompt & wait again

The user input is always just one string. We use the 'readline' function to get it.

We chose to execute and expand with a list of commands. This makes the logic for the execution much easier to understand and it's very practical for the expansion as well.

We did not do the bonus, so our minishell doesn't handle '&' or '*'. 

This is what we do with the input:

- Tokenize

- Create commands from the tokens

- Execute with the list of commands
  
- Save the return value for the next round

---------------------------------------------------------------------------

Some parts of this project that can be done pretty independently:
1. Tokenization
2. Creation of commands + execution
3. Expansion
4. Builtin functions
5. Reading loop (prompt, readline, history)
6. Environment

Other parts, like the handling of signals, affect other things such as the reading loop and the execution. That's why I would recommend to do them at the end of the project, when everything already works.

I recommend the tokenization and creation of commands + execution to be done by the same person because in order to create the tokens, you need to know what are the things that are going to be necessary for the execution of the commands. 

However, If one of you already knows how they are gonna execute and has a very specific structure, then the tokenizing can be done by the other person and there should be no problem. 

Communicating these things in the begining (and always) is very important, and if none of you are sure of how to do things (which is normal in the early stages), you could end up doing unnecessary stuff or complicated code, which is not the end of the world, but I really appreciated learning about the simpler ways to make it work.

Our list of tokens only has two types/variables:
- Word:  anything except space/tab
- Token:  |, <<, <, >, >>

We have an enum that specifies which type of token it is: pipe, heredoc, append, etc, but that's it.
With this information we are able to create a list of commands (each with their own list of redirections) and execute correctly. 

Understanding Bash
 -------------------------------------------------------------------------------
The step to go from tokens to commands is actually pretty easy once you know the patterns/logic that bash uses to analyze the string.

If you have already done tokenization you must already know how bash separates words from tokens, how it handles spaces, etc, so this step is similar and much easier to write in code.

Here are some basic patterns that are ALWAYS true:
- After a redirection token (<, <<, >, >>) the next thing to the right is always going to be the filename/limiter in the case of the heredoc -> in any case, A WORD

    examples:

        1. < infile      ✓
     
        2. infile <      X (this is a syntax error because after the token there is nothing, and it expects a filename)

    In the 2nd example, 'infile' would be interpreted as a command

- The '|' needs to be preceded by a word (not a token) and needs to have either a word or another redirection token right after.

    You could want to handle what bash does when you dont put anything after the '|' and there is something before; we didn't, but that's up to you.

- Each command is separated by a '|'. If there is no pipe, there is only 1 command.
  
  example1:

         ls libft srcs/execution -l
  
  This is all 1 command. It is 'ls' because it's the first word to appear. Whatever is in the first position is going to be the command.
  Whatever comes after are all arguments or flags of that command. The flag doesn't have to be right next to the command and it should still work.
  We simply have to send all the arguments in the char **argv to 'execve' and it will handle everything for us.

  example2:

          -a ls libft srcs/execution -l
  
  The command in this case is '-a'.

  example3:

          << lim > outfile cat > outfile2 -e
  
  The command here is 'cat' because all the other words before 'cat' are preceded by a redirection token, which means they are the filenames of those redirections.

Once you are familiar with bash's behaviour and how the terminal works, you'll find that it's a simple process.  

About execution
 -------------------------------------------------------------------------------
As we execute with a list of commands, the idea is simple: 

    while(command exists)
    {
      execute command
      move to the next one
    }

Before we try to execute we need to ask ourselves some questions:
- How many commands do we have?
  
- Are they a builtin or a normal cmd?

The response to this questions is gonna determine whether we need a fork or not, and how we are gonna handle the file descriptors of redirections and pipes

TO KEEP IN MIND
- Builtins should be executed in the main process because they are handmade functions.

  The export builtin for example, modifies the environment so if we did it in a child process, we would never see the changes!
  
  This is exactly what happens when we have multiple commands and it's OKAY, it's suposed to be like that.
- Whenever you use fork(), you are duplicating the information of the main process; this includes the filedescriptors, so be sure to use ONLY the necessary and get rid of the ones you no longer need in the parent and in the child processes.
  
- If you have 1 command, it's a builtin and has redirections, if you handle the filedescriptors in the parent, remember to set the std_in and std_out files to 0 and 1 again for the next line!

I recommend thinking of always getting the information you need in the precise moment that you need it. Not before.

For example, in the case where you have a simple command like 'ls', which is not a builtin, we open the redirection files INSIDE of the child process, not before because the parent doesn't need them and we would have to close them later. We handle the errors inside the child process. This may be more difficult if you do the bonus.

This type of thing seems silly/obvious, but it really helps and it's a good practice.
  
Before sending the command to 'execve', we need to do:
1. Expansion of the char **argv (of each command)
2. Find command path
3. Expansion of the filename of redirections (for each command)
4. Redirections
   
   4.1. Opening the files

   4.2. Actual dup2() redirections (only 2: one for std_in, one for std_out)
   
As you can see, the expansion is done the exact moment before we need to use the argv and files, respectively.

In the case of the multiple command, the loop of execution would look like this:

    while(command)
    {
      prep_cmd: expand argv, find path, create pipe fds;
      fork();
      if (you are child)
      {
        open_files;
        redirect;
        if (you are builtin)
        {
          do_builtin;
          exit();
        }
        execve;
      }
      -> next command
    }

Obviously every function needs to be protected, but this is basically it.

Redirections, pipes & fds
------------------------------------

A good start is to establish communication of multiple commands with the pipes. We are not going to think about redirections yet.

Let's imagine we have these 3 commands:  

 ls | cat | grep libft

We know that we need a place to store the fds that the pipe() function is going to modify.
In our exec structure, we declared it like so:
 
      int  pipe_end[2];

These are the 'boxes'/memory address we are going to send to the pipe function as argument. The function will change the value inside those memory addresses.

Each time we go to prep_cmds, we ask ourselves: is there a next command?

-> If there is, we use the pipe(exec->pipe_end)

-> If not, it means we are the last command so there is no need for another pipe

We start on the first command, we have a next, so we have the fds in exec->pipe_end.

    pipe_end[0] for reading
    pipe_end[1] for writing
    
For the first command we don't need the reading fd, cause we are simply going to write the result of 'ls'. 'cat' however, will need the reading fd of this first pipe to read what 'ls' has written.
The second command will create the 2nd pipe and will need only the writing part. 'grep' will need only the reading fd of the second pipe because it's going to write in the terminal (std_out -> 1).

If we think about it, for the execution of multiple commands

 - The first commands needs the writing end of the actual pipe
   
 - The commands in the middle need the reading end of the last pipe + the writing end of the actual pipe

 - The last command needs the reading end of the last pipe











Expansion ✨
------------------------------------

Arguably the most difficult part of this project. It requires having things very very clear and unlike with the creation of commmands, trying to understand bash's behaviour is really not that easy. This is how I aproached it.


Files and general organization
------------------------------------


Error managment
------------------------------------



