# YoutubeFinder
Each youtube video contain a unique ID :
![image](https://user-images.githubusercontent.com/54883972/194720586-b7a4e65f-eac0-43dc-a6a4-fc4be3ed4176.png)


The ID is made of 11 characters, and there are 64 different characters,
so there is 64^11 = 73 786 976 294 838 206 464 different ID combinations.
In 2017, more than 7 billion videos were on Youtube, so there is around 1 on 10 millions chance to find an already used ID.

 - Goal : Find a video.
 
 # How
 
 All clients are connecting to a server. The server generate 9 of the 11 characters needed to make a Youtube ID.
 The client check all 4096 ids to see if there is a video.
 
 
