# YoutubeFinder
Each youtube video contain a unique ID :
![image](https://user-images.githubusercontent.com/54883972/194720586-b7a4e65f-eac0-43dc-a6a4-fc4be3ed4176.png)


The ID is made of 11 characters, and there are 64 different characters,
so there is 64^11 = 73 786 976 294 838 206 464 different ID combinations.
In 2017, more than 7 billion videos were on Youtube, so there is around 1 on 10 billions chance to find an already used ID.

 - Goal : Find a video.
 
 # How
 
 All clients are connecting to a server. The server generate 9 of the 11 characters needed to make a Youtube ID.
 The client check all 4096 ids to see if there is a video.
 
 
 # Erratum
 
I though that the formula to find the total number of possibilites was 64^11 but it isn't.
We need to use the permutation formula to find the right number :

![image](https://user-images.githubusercontent.com/54883972/209566426-13cf0e06-bb09-422f-8ed2-e1005b88bb50.png)

And that leave us with a total of 29 681 964 103 912 243 200 id possible.
If there was 10 billions videos on Youtube, that would give us 1 / 2 968 196 410 to find an id with a video associated to it.

We also need to take into account that a lot of videos are just in private, so there wont be any way to see them.

Last time i worked on this software, i was able to make 4096 id test every minutes with ~30Mb/s.
I was able to start the software multiple times and with the fiber optic, i was at ~50k test every minutes.
So arround 3 millions every hours.
I would need ~1000 hours to have those 2.968 billions.
So. What could be done ?

1) Find more people to use the software. If we were 1000, we would only need 1 hour ?
2) Find a way to test faster ? For that we really need to understand how the software work at the moment:
   We download the entire page, and check if there is the "status":"ERROR" string.
   Maybe we could stop the download once the string as been found ? That could give us a little bit of speed ?
   
Those are the ways i can think of. They're may be more, so if someone goes through this readme and think of something, please tell me !

This was my first attempt of making a software using Qt. They are plenty of things that i could do to make it better and I'll probably work on it one day !
