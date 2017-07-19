	identification	division.
	program-id.	'binary'.

	environment	division.
	input-output	section.
	file-control.
      *		select	INFILE	assign to	INFILE
      *			organization	is	sequential.
      *		select	OTFILE	assign to	OTFILE
      *			organization	is	sequential.

	data		division.
	file		section.
      *	fd	INFILE.
      *		record	is varying in size
      *		depending on	in-rec-ll.
      *	01	in-rec.
      *	    03	filler		occurs 32700 pic x(01).
      * 01	in-rec.
      *	    03	filler		pic	x(16).

      *	fd	OTFILE.
      *		record	is varying in size
      *		depending on	ot-rec-ll.
      *	01	ot-rec.
      *	    03	filler		occurs 32700 pic x(01).
      * 01	ot-rec.
      *	    03	filler		pic	x(16).

	working-storage	section.
	01	in-rec-ll	pic	9(04)	comp.
	01	ot-rec-ll	pic	9(04)	comp.

	01  wk-comp.
	  03	comp04		pic	s9(04)	comp.
	  03	filler		pic	x(02).
	  03	comp09		pic	s9(09)	comp.
	  03	comp18		pic	s9(18)	comp.

	01  wk-binary.
	  03	binary04	pic	s9(04)	binary.
	  03	filler		pic	x(02).
	  03	binary09	pic	s9(09)	binary.
	  03	binary18	pic	s9(18)	binary.

	01  wk-comp5.
	  03	comp504		pic	s9(04)	comp-5.
	  03	filler		pic	x(02).
	  03	comp509		pic	s9(09)	comp-5.
	  03	comp518		pic	s9(18)	comp-5.

	procedure	division.
	main		section.

		display "hello world!!".
		move	4660		to	comp04
						binary04
						comp504.

		move	19088743	to	comp09
						binary09
						comp509.

		move	4886718336	to	comp18
						binary18
						comp518.

      *		open	output OTFILE.
		move	16	to	ot-rec-ll.

      *		write	ot-rec	from wk-comp.
      *		write	ot-rec	from wk-binary.
      *		write	ot-rec	from wk-comp5.

      *		close	OTFILE.
		exit-program.
