  Minesweeper                                                                                                                       
                                                                                                                                    
  Project Description                                                                                                               
                                                                                                                                    
  A classic Minesweeper game built in C++ using the SFML graphics library. Features a complete gameplay experience with a welcome   
  screen, interactive game board, timer, mine counter, and a persistent leaderboard system.                                         
                                                                                                                                    
  Features:                                                                                                                         
  - Welcome Screen - Name entry for leaderboard tracking                                                                            
  - Interactive Game Board - Left-click to reveal tiles, right-click to flag bombs                                                  
  - Timer & Mine Counter - Track your progress in real-time                                                                         
  - Leaderboard - Persistent high scores saved between sessions                                                                     
  - Debug Mode - Reveal mine locations for testing                                                                                  
  - Recursive Reveal - Empty tiles automatically reveal neighboring tiles                                                           
                                                                                                                                    
  Game Rules:                                                                                                                       
  - Reveal all non-bomb tiles to win                                                                                                
  - Flag all bombs and reveal all safe tiles                                                                                        
  - Clicking a bomb ends the game                                                                                                   
                                                                                                                                    
  Screenshots                                                                                                                       
                                                                                                                                    
  Coming soon                                                                                                                       
                                                                                                                                    
  How to Run It                                                                                                                     
                                                                                                                                    
  Prerequisites                                                                                                                     
                                                                                                                                    
  - C++ compiler with C++11 support (MinGW recommended)                                                                             
  - SFML 2.5.1 (bundled)                                                                                                            
                                                                                                                                    
  Build                                                                                                                             
                                                                                                                                    
  Open the project in CLion and build, or compile manually with your preferred method linking SFML libraries.                       
                                                                                                                                    
  Usage                                                                                                                             
                                                                                                                                    
  Run the executable and enter your name to start playing.                                                                          
                                                                                                                                    
  Technologies Used                                                                                                                 
                                                                                                                                    
  - C++                                                                                                                             
  - SFML 2.5.1                                                                                                                      
  - MinGW                                                                                                                           
  - CLion                                                                                                                           
                                                                                                                                    
  What I Learned                                                                                                                    
                                                                                                                                    
  - Game Loop Architecture - Implementing a render/update cycle that handles game state, user input, and screen rendering in a      
  continuous loop                                                                                                                   
  - Object-Oriented Component Design - Creating modular UI components (Tile, Button, Timer, Counter, TextField, UIRenderer) that    
  encapsulate their own state and behavior                                                                                          
  - Recursive Algorithms - Implementing cascade tile reveal where clicking an empty tile recursively reveals all adjacent empty     
  tiles                                                                                                                             
  - Event Handling - Processing mouse clicks and keyboard input to drive game interactions and state changes                        
                
