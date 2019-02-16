#Client-Server Socket IPC for Student Eligibility Computation

*	Client and Server should perform IPC through Network using Sockets
*	Client should send the student’s registration number (given by user) to the Server
*	Server should fetch the given student’s Component-1 (TermTest-1, TermTest-2, TermTest-3, Assignment-1, Assignment-2) marks and Attendance
*	Server should return the component-1 marks and attendance to client
*	Client should compute the eligibility of student from the received data based on following conditions:
    ..1.	The total of component-1 i.e.  best two of (TermTest-1, TermTest-2 & TermTest-3 marks), Assignment-1 and Assignment-2 marks should be greater than or equal to 40
    ..2.	Attendance should be greater than or equal to 80%
    ..*     A student is said to be eligible if both conditions are true else, he/she is non-eligible
*   Client should tell the student the reason of non-eligibility (i.e. either low attendance or less marks)
