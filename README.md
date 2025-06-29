# 42_minishell GUIDE
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

Execution
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

Pipes & redirections
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

So, how do we store the fds of the previous pipes?

I didn't want to put a 'previous' pointer in my cmd list, so instead I added two boxes for each node:

    int  fd_in;
    int  fd_out;
    
Every time there is a next command, I save the writing end of the actual pipe in the 'fd_out' of the actual command.
Then I save the reading end of the actual pipe as the 'fd_in' of the NEXT command.

It looks like this:

    if (cmd->next)
  	{
  		pipe(exec_info->pipe_end);
  		cmd->fd_out = exec_info->pipe_end[1];
  		cmd->next->fd_in = exec_info->pipe_end[0];
  	}

This way, each cmd has in their structure the necessary information in fd_in and fd_out to redirect to the corresponding pipes.

Once that works, we can implement redirections pretty easily.

------------------------------------------------------------

The order of redirections is very important. The heredoc is a special case we'll see later.

Most of the times, the user is going to input commands that make sense, so the redirections should not affect the pipe fds, but sometimes the combination of redirections and pipes will make the pipes useless and your minishell should handle that just like bash does.

Examples:

          < infile cat | grep hello > outfile   
      
   This makes sense, in fd_in of 'cat' we'll have the fd of 'infile' and in fd_out of 'grep' we'll have the fd of 'outfile'

          < infile cat > outfile | grep hello    
          
   In this last case the redirections override the pipe. The commands are still going to be executed, they simply won't be connected.
   Therefore, 'cat' will write in 'outfile' and 'grep' won't receive any input. 
  
  It's very important to close the fds properly because in this case we could end up with the terminal forever waiting for input. I think 'grep' should realize that the pipe_end[1] has been closed and therefore the pipe_end[0] is also closed, and that will cause execve to exit. 
  I believe this type of thing happens with very specific commands like 'grep' and 'cat' because they read from std_in line by line, and also because of the way the pipe() works.


In any case, we have a list of redirections inside each command, where we store by order the fd of the files.

We go through our list like this:

    while(redir exists)
    {
      open file
      if (there was a previous fd_in or fd_out)  // could be from previous redirection of the same type 
                                                 // could be from pipe
      {
        close(previous fd);
      }
      save file fd in fd_in or fd_out
      -> next redir
    }
            
Everytime we find a redirection of type INPUT or HEREDOC we open the file and put that fd into the fd_in.

Everytime we find a redirection of type OUTPUT or APPEND we open the file and put that fd into the fd_out.

This way, when we do dup2(), we only redirect once with the fd_in and once with the fd_out.

HEREDOC

The heredoc is special because it needs to be done before we start execution.

I chose to do it using a pipe, writing in the writing end and storing the reading end in the 'fd' variable in the redirections list.

This means that when we arrive to the execution part where we open the files to redirect, we won't have to open anything for the heredoc and we'll simply redirect.

Take a good look at how bash handles errors while opening the files, the order of the messages, and the files it creates.


Expansion ✨
------------------------------------

Arguably the most difficult part of this project. It requires having things very very clear and unlike with the creation of commmands, trying to understand bash's behaviour is really not that easy. This is how I aproached it.

THINGS TO KEEP IN MIND:
- Expansion is done in a function that receives a char *string and returns a char **array

  This char **array is separated accordingly

- We don't handle what happens when someone exports a variable with quotes inside. (ex: export test="hola'quetal")

  Our program doesn't segfault but it also doesn't behave like bash.
  We do handle correctly variables with spaces inside.

- After actual expansion, we are not done. We'll need to handle error messages + the char **argv sent to 'execve'.

- Expansion of the char **argv and the filename (redirection file) will be done independently. We use the same function and then we manage its results.

This is how the loop to expand the argv can look like:

    char **aux = NULL; 
    char **new_argv = NULL; 
    while(command->argv[x])
    {
  		i = 0;
  		aux = expansion(command->argv[x]) // we send char *, it returns char **
  		while (aux && aux[i])
  		{
                    function to add aux[i] to new_argv
  			i++;
  		}
      x++;
    }
    
    // the idea is that new_arg will replace argv
    
    free_array(command->argv);
    command->argv = new_arg;
    
This is just one way of doing it, you can choose to store the char ** however you like.

Steps:
1. Divide initial string into sections ( , ", ')
2. Divide each section between expandible and non-expandible part
3. Search & replace the expandible part
4. Join expandible part with non-expandible
5. Join all sections
6. Divide by spaces if necessary
7. Remove quotes

It may seem a little confusing, but I only worked with strings and char **arrays, I didn't create a structure or list for this process.

This is a visual representation of the general process:

![image](https://github.com/user-attachments/assets/68db58f2-3834-4a63-b66f-0fb4b5b3ecb1)
<ins>Step 1</ins>

The first division creates sections composed of strings without quotes, strings with double quotes, and strings with simple quotes.

The only possible spaces that we can receive will be **inside** quotes, therefore they DON'T affect the char **argv. It's all still considered 1 string. Later we'll see a case with separation of the argv.

<ins>Step 2</ins>

Files and general organization
------------------------------------



