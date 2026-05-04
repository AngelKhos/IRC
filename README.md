_This project has been created as part of the 42 curriculum by authomas, gchauvet._

# FT_IRC


## DESCRIPTION

Ft_IRC is a basic server following the IRC protocol and meant to work with a chosen IRC client (for this project we chose **hexchat** as our reference IRC client). It has to be made using online documentation ( the holy RFC) and need to have the same behavior as any official IRC server, however, only a handful of features need to be implemented and working. 

## INSTRUCTION

### Compiling and installation

In order to be able to launch the project on your machine, you must clone the repository from github or the vogosphere.

The machine must have the compiler  **c++**  linked to  **clang 12.0.1 / gcc 13.3.0 or newer**  for optimal compilation and the  **build-essentials**  apt package installed.

You can install said packages with the command :

```
sudo apt update
sudo apt install build-essentials
```

And you can check your compiler version and the cc linkage with the commands:  
`c++ --version`

To compile the executable, run the command:  
`make`

To delete easily the compilation files, run the command:  
`make clean`

To delete the executable, run the command:  
`make fclean` 

To recompile the whole project without worrying about removing the compilation files, run the command:  
`make re`

## Lauching the project

Now that the project is compiled and ready to be launched, you can now run it:  
`./ircserv <port> <password>`  

## RESSOURCES

The ressources used for this projects are:

https://www.rfc-editor.org/rfc/rfc1459.html

https://www.rfc-editor.org/rfc/rfc2812.html

Peer learning

AI had a very minimal use in the project. It allowed us to have a quick access to documentation on functions (man).