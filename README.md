<h1>War Card Game</h1>

<b>DESCRIPTION:</b>
<p>This is a very simply card game- War, where 2 or more cards are compared. The highest card wins
   the lower card(s). Losing all cards loses the game.</p>
<p>My purpose in making this game is to apply my card-game-framework repository and learn how to
   use the SFML framework more effectively. Once I complete this project, I should be able to apply
   the things learned to a more compicated card game.</p>

<b>DEPENDENCIES:</b>
<ul><li>C++ 11</li>
    <li>SFML (Get it here: https://www.sfml-dev.org/index.php)</li></ul>

<b>COMPILE INSTRUCTIONS (CMake):</b>
<ol><li>Create "Build" folder in the code directory</li>
    <li>Change directory into the "Build" folder</li>
    <li>Run command: '<b>cmake ..</b>' (auto configure) or '<b>ccmake ..</b>' (for manual configuring)<br> 
    	If manual, configure anything needed but you should only need to add "Debug" to the blank line.
        Enter "c" to create a cmakeList.txt file. Enter "g" to finish generating and exit the gui.</li>
    <li>After the cmake files have been generated in the "Build" folder, run the command: '<b>make</b>'. 
        This will create a "Main.out" executable file in the source folder.</li>
    <p>*Note: Only step 4 will be needed for repeat compiles once the Build folder is created and 
        initialized. There are also scripts to assist with compiling and cleaning garbadge files.</p></ol>

<b>COMPILE INSTRUCTIONS (g++):</b>
    <p>If CMake isnt working, you can use the 'compileAndRun.bash' script. Just run as is.</p>

<b>UML DIAGRAM</b>

![](Images/UML_Diagram.png)
