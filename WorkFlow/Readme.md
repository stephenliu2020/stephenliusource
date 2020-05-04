This is the code repository for work flow project. It creates a work flow like below:
It is used for robot to pick and place an object to a container.

                      ---------
                      | start |
                      ---------
                          |
                      -----------
                      | scatter |
                      -----------
                          |
                         / \
                        /   \
                       /Motor\  n  -------
                       \Run? /-----| end |
                        \   /      -------
                         \ /
                          |y
                     ----------------
                     | feeding stop |
                     ----------------
                          |
                     -------------
                     | ARM pick  |
                     -------------
                          |
                         /  \
                        /    \
                       /Detect\  n  -------------
                       \Powder/ ----|Reject Deck |
                        \    /      --------------
                         \  /
                          |y
                     -------------
                     | ARM pick  |
                     -------------
                           |
                          / \
                         /   \
                        /     \
                       /Detect \  n  ----------------
                       \Compact/ ----|Reject Compact|
                        \     /      ----------------
                         \   /
                          \ /
                           |y
                     -------------
                     | ARM place |
                     -------------
                           |
                       -------
                       | end |
                       -------

It can run under QT environment such as QT5. It can run under Linux and Windows environment.
Install QT creator. Then use QT creator to open WorkFlowControl.pro.
After build, it will generate a binary executable file. Run it and it will show work flow diagram as above.
Each rectangle stands for a button. Clicking any button can stop and resume the work flow. 
                          



