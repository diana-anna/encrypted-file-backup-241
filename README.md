# 241HonorsProject

Project Proposal: Claire Boyan and Diana Kowalski

Language: C

Overview:

We plan to develop our own file backup system, similar to Dropbox, or Google Drive without the editing capabilities. This system will encrypt the files that are uploaded with keys available only to the client (the server can only view encrypted files).
 
Purpose:

We read an article by a man who analyzed exactly what information Google and Facebook keep on users.


"This is my Google Drive, which includes files I explicitly deleted including my résumé, my monthly budget, and all the code, files and websites I’ve ever made, and even my PGP private key, which I deleted, that I use to encrypt emails."


These companies had 5.5 GB of data from the author's usage alone. This fact, combined with the knowledge of servers that do encrypt personal data (like WhatsApp), made us aware that these companies have the ability to encrypt data but don't always utilize it. Not only do these companies use user data to target ads, but this lack of encryption also leaves people like journalists and activists exposed to government surveillance and interference with their professions. We want to create a secure file uploading server that utilizes strong encryption methods.


Expected workload distribution:

Since we won't be writing our own encryption methods, the majority of the work will be devoted to the uploading and sharing of files on a secure network. We plan to evenly distribute the workload, including writing code and testing it. As we delve more into this project, we will decide how to split the coding of functions and test writing. If there are sets of functions that rely directly on each other, each set can be like a separate workload, and we would then split those sets up.

Projected project milestones

* Compiling the simple client/server example from the coursebook section on networks
* Establishing a connection (using NCAT)
* Getting files and other data to send
* Error checking/signal handling (EAGAIN/EPIPE)
* Getting files/data to encounter an issue and then resolve that issue independent of the user
* Finding and implementing an encryption library
* Sharing process


Midterm Presentation:

We expect to definitely have an established server/client connection. We plan on also having the ability to send data between machines.

Final Presentation:

We expect to have done everything in the project milestones, at the very least we will be able to send and share encrypted files on a network and utilize robust error checking/signal handling.


Expected challenges:

- Some of the project developments are covered under topics later in the semester (networking, filesystems, etc.)
- Figuring out what all the startup code means for networks
- Dealing with errors we have never seen before and figuring out how to solve them
- Finding resources for the server and encryption algorithms
- Testing
