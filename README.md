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

Communicating these things in the begining (and always) is very important, and if none of you are sure of how to do things (which is normal in the early stages), you could end up doing unnecessary stuff or complicated code, which is not the end of the world, but I would really appreciated learning about the simple ways to make it work.

Our list of tokens only has two types/variables:
- Word:  anything except space/tab
- Token:  |, <<, <, >, >>

With this information we are able to create a list of commands (each with their own list of redirections) and execute.

Understanding Bash
 -------------------------------------------------------------------------------
The step to go from tokens to commands is actually pretty easy once you know how bash behaves. When you know the patterns/logic that it uses to analyze a string, everything becomes much more clear and easy.

Here are some basic patterns that are ALWAYS true:
- After a redirection token(<, <<, >, >>) the next thing to the right is always going to be the filename/limiter in the case of the heredoc -> in any case, A WORD

    examples:

        1. < infile      ✓
     
        2. infile <      X (this is a syntax error because after the token there is nothing, and it expects a filename)

    In the 2nd example, 'infile' would be interpreted as a command

- The '|' needs to be preceded by a word (not a token) and needs to have either a word or another redirection token right after.

    You could want to handle what bash does when you dont put anything after the '|' and there is something before; we didn't, but that's up to you.


About execution
 -------------------------------------------------------------------------------
As I execute with a list of commands, the idea is simple: while(command exists){execute command, move to the next one}

I do the redirections(opening files) just before I execute



