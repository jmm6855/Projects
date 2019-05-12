# FILE:		connect4.asm
# AUTHOR: 	Jacob Manelius
#
# DESCRIPTION:
#	In this project you can play a functional version of connect 4
#	by entering numbers with the keyboard against an opponent
#
# ARGUMENTS:
#	None
#
# INPUT:
#	An integer through the keyboard
#
# OUTPUT:
#	A board display while playing connect 4
#
# REVISION HISTORY:
#	April	15, 19	-forced to restart the program after accidental deletion
#

#
# CONSTANT DECLARATIONS
#
PRINT_INT	= 1			# code for syscall to print integer
PRINT_STRING	= 4		# code for syscall to print a string
READ_INT	= 5			# code for syscall to read an int
MAX_ROWS	= 6			# the number of rows on the board
MAX_COLS	= 7			# the number of columns on the board
PLAYER_1	= 1			# the number to represent the first player
PLAYER_2	= -1		# the number to represen the second player

#
# DATA DECLARATIONS
#
	.data
array:
	.space	44		# initialize an array of 44 bytes
welcome_msg:
	.ascii	"   ************************\n"
	.ascii	"   **    Connect Four    **\n"
	.asciiz	"   ************************\n"
player_1_move:
	.asciiz	"Player 1: select a row to place your coin (0-6 or -1 to quit):"
player_2_move:
	.asciiz	"Player 2: select a row to place your coin (0-6 or -1 to quit):"
illegal_column:
	.asciiz	"Illegal column number.\n"
full_column:
	.asciiz	"Illegal move, no more room in that column.\n"
tie_msg:
	.asciiz	"The game ends in a tie.\n"
player_1_win_msg:
	.asciiz	"Player 1 wins!\n"
player_2_win_msg:
	.asciiz	"Player 2 wins!\n"
player_1_quit_msg:
	.asciiz	"Player 1 quit.\n"
player_2_quit_msg:
	.asciiz	"Player 2 quit.\n"
game_board:
	.ascii	"\n   0   1   2   3   4   5   6\n"
	.ascii	  "+-----------------------------+\n"
	.ascii	  "|+---+---+---+---+---+---+---+|\n"
	.ascii	  "||   |   |   |   |   |   |   ||\n"
	.ascii	  "|+---+---+---+---+---+---+---+|\n"
	.ascii	  "||   |   |   |   |   |   |   ||\n"
	.ascii	  "|+---+---+---+---+---+---+---+|\n"
	.ascii	  "||   |   |   |   |   |   |   ||\n"
	.ascii	  "|+---+---+---+---+---+---+---+|\n"
	.ascii	  "||   |   |   |   |   |   |   ||\n"
	.ascii	  "|+---+---+---+---+---+---+---+|\n"
	.ascii	  "||   |   |   |   |   |   |   ||\n"
	.ascii	  "|+---+---+---+---+---+---+---+|\n"
	.ascii	  "||   |   |   |   |   |   |   ||\n"
	.ascii	  "|+---+---+---+---+---+---+---+|\n"
	.ascii	  "+-----------------------------+\n"
	.asciiz	  "   0   1   2   3   4   5   6\n\n"
#
# MAIN PROGRAM
#
	.text
	.align	2
	.globl	main
main:
	addi	$sp, $sp, -40	#space for return address
	sw		$ra, 32($sp)	#store the ra on the stack
	sw		$s7, 28($sp)
	sw		$s6, 24($sp)
	sw		$s5, 20($sp)
	sw		$s4, 16($sp)
	sw		$s3, 12($sp)
	sw		$s2, 8($sp)
	sw		$s1, 4($sp)
	sw		$s0, 0($sp)
start_main:
	
	move	$t0, $zero		# counter
	addi	$t1, $zero, 44	# limit
zeroing_array:
	slt		$t2, $t0, $t1	# while t0 < t1, t2 = 1
	beq		$t2, $zero, array_zeroed

	la		$t5, array		# load the array
	add		$t5, $t5, $t0	# set the index
	sb		$zero, 0($t5)	# set the value to 0
	addi	$t0, $t0, 1		# increment counter
	j		zeroing_array

array_zeroed:

	la		$a0, welcome_msg	# print welcome message
	jal		print_string

	la		$a0, game_board		# print initial game board
	jal		print_string

	move	$s0, $zero			# set player 1 as first player
	addi	$s0, $s0, PLAYER_1

ask_for_number:
	slt		$t0, $s0, $zero
	bne		$t0, $zero, ask_player_2
	
	la		$a0, player_1_move		# ask player 1 for a number
	jal		print_string

	la		$v0, READ_INT
	syscall	
	move	$s1, $v0				# get the input from the player

	move	$t0, $zero
	addi	$t0, $t0, -1
	beq		$t0, $s1, player_1_quit	# a player quit the game

	slt		$t0, $s1, $zero
	bne		$t0, $zero, input_error	# input was less than 0
	move	$t1, $zero
	addi	$t1, $t1, MAX_COLS
	addi	$t1, $t1, -1
	slt		$t0, $t1, $s1
	bne		$t0, $zero, input_error	# input was greater than number of columns

	j		got_number				# input was good

ask_player_2:
	la		$a0, player_2_move		# ask player 2 for a number
	jal		print_string

	la		$v0, READ_INT
	syscall	
	move	$s1, $v0				# get the input from the player

	move	$t0, $zero
	addi	$t0, $t0, -1
	beq		$t0, $s1, player_2_quit	# a player quit the game

	slti	$t0, $s1, 0
	bne		$t0, $zero, input_error	# input was less than 0
	move	$t1, $zero
	addi	$t1, $t1, MAX_COLS
	addi	$t1, $t1, -1
	slt		$t0, $t1, $s1
	bne		$t0, $zero, input_error	# input was greater than number of columns

	j		got_number				# input was good

input_error:
	la		$a0, illegal_column
	jal		print_string
	j		ask_for_number

player_1_quit:
	la		$a0, player_1_quit_msg		# player 1 quit the game
	jal		print_string
	j		main_done					# end the program

player_2_quit:
	la		$a0, player_2_quit_msg		# player 2 quit the game
	jal		print_string
	j		main_done					# end the program

got_number:
	move	$a0, $s0
	move	$a1, $s1
	jal		place_piece					# place the players piece
	slt		$t0, $v0, $zero
	bne		$t0, $zero, full_error		# bad input, try again

	move	$a2, $a1					# move the chosen column to a2
	move	$a1, $v0					# move the chosen row to a1

	jal		check_vertical_win			# check for a win vertically
	bne		$v0, $zero, win_achieved	# current player won

	jal		check_horizontal_win		# check for a win horizontally
	bne		$v0, $zero, win_achieved	# current player won

	jal		check_diagonal_win			# check for a win on the diagonals
	bne		$v0, $zero, win_achieved	# current player won

	jal		check_draw
	bne		$v0, $zero, draw			# check if the game drew

	la		$a0, game_board				# no winner, print the board, continue
	jal		print_string

	slti	$t0, $s0, 0
	bne		$t0, $zero, switch_to_p1

	move	$s0, $zero
	addi	$s0, $s0, PLAYER_2

	j		ask_for_number		# return to the top of the loop

switch_to_p1:
	move	$s0, $zero
	addi	$s0, $s0, PLAYER_1	# switch the active player to player 1

	j		ask_for_number		# return to the top of the loop
	

win_achieved:
	la		$a0, game_board		# print the board
	jal		print_string

	slt		$t0, $s0, $zero
	bne		$t0, $zero, player_2_win

	la		$a0, player_1_win_msg	# print the winning message
	jal		print_string
	j		main_done				#jump to the end

player_2_win:
	la		$a0, player_2_win_msg	# print the winning message
	jal		print_string
	j		main_done				# jump to the end

full_error:
	la		$a0, full_column		# column is full, print error
	jal		print_string
	j		ask_for_number			# get a new number

draw:
	la		$a0, tie_msg			# print the tie pessage
	jal		print_string

main_done:
	lw		$ra, 32($sp)
	lw		$s7, 28($sp)
	lw		$s6, 24($sp)
	lw		$s5, 20($sp)
	lw		$s4, 16($sp)
	lw		$s3, 12($sp)
	lw		$s2, 8($sp)
	lw		$s1, 4($sp)
	lw		$s0, 0($sp)
	addi	$sp, $sp, 40	#clean up the stack
	jr		$ra
	

#
# Name:		print_string
#
# Description:	This routine prints out a string pointed to by a0
# Arguments:	a0:	a pointer to the string to print
# Returns:	nothing
#
print_string:
	li	$v0, PRINT_STRING
	syscall		# print a0

	jr	$ra


#
# Name:		place_piece
#
# Description:	This routine places a piece on the selected column
#				with the marker of the given player
# Arguments:	a0:	current player
#				a1:	the chosen column
# Returns:	v0:	the row the piece was placed in, -1 if column was full
#
place_piece:
	addi	$sp, $sp, -40	#space for return address
	sw		$ra, 32($sp)	#store the ra on the stack
	sw		$s7, 28($sp)
	sw		$s6, 24($sp)
	sw		$s5, 20($sp)
	sw		$s4, 16($sp)
	sw		$s3, 12($sp)
	sw		$s2, 8($sp)
	sw		$s1, 4($sp)
	sw		$s0, 0($sp)

	move	$s0, $zero				# index of array to place piece
	addi	$t5, $zero, MAX_COLS	# size of rows

	addi	$t0, $zero, MAX_ROWS
	addi	$t0, $t0, -1			# set up a counter

placement_loop:
	la		$s1, array				# get the array address

	slt		$t1, $t0, $zero
	bne		$t1, $zero, error_done	# the column was full, invalid placement

	mult	$t0, $t5		# row * rowSize
	mflo	$s0				# s0 = row * rowSize
	add		$s0, $s0, $a1	# s0 = row * rowSize + col

	add		$s1, $s1, $s0
	lb		$s2, 0($s1)		# s2 = value at current index of array

	beq		$s2, $zero, placement_found	# the current index is unoccupied

	addi	$t0, $t0, -1
	j		placement_loop

placement_found:
	slti	$t7, $a0, 0
	bne		$t7, $zero, set_player_2
	
	addi	$t9, $zero, 1
	sb		$t9, 0($s1)

	la		$s3, game_board		# get the game board
	addi	$s3, $s3, 97
	move	$t4, $zero
	addi	$t4, $t4, 64		# row size
	mult	$t0, $t4
	mflo	$t8
	add		$s3, $s3, $t8
	move	$t4, $zero
	addi	$t4, $t4, 4
	mult	$t4, $a1
	mflo	$t8
	add		$s3, $s3, $t8

	move	$t4, $zero
	addi	$t4, $t4, 88		# 88 is ascii for X
	sb		$t4, 0($s3)			# save an X to the board

	j		placing_done

set_player_2:
	addi	$t9, $zero, -1
	sb		$t9, 0($s1)

	la		$s3, game_board		# get the game board
	addi	$s3, $s3, 97
	move	$t4, $zero
	addi	$t4, $t4, 64		# row size
	mult	$t0, $t4
	mflo	$t8
	add		$s3, $s3, $t8
	move	$t4, $zero
	addi	$t4, $t4, 4
	mult	$t4, $a1
	mflo	$t8
	add		$s3, $s3, $t8

	move	$t4, $zero
	addi	$t4, $t4, 79		# 79 is ascii for O
	sb		$t4, 0($s3)			# save an O to the board

	j		placing_done

placing_done:
	move	$v0, $t0		# save the row to the return register
	j		done
error_done:
	move	$v0, $zero		# error return to the return register
	addi	$v0, $v0, -1
	j		done
done:
	lw		$ra, 32($sp)
	lw		$s7, 28($sp)
	lw		$s6, 24($sp)
	lw		$s5, 20($sp)
	lw		$s4, 16($sp)
	lw		$s3, 12($sp)
	lw		$s2, 8($sp)
	lw		$s1, 4($sp)
	lw		$s0, 0($sp)
	addi	$sp, $sp, 40	#clean up the stack
	jr	$ra


#
# Name:		check_vertical_win
#
# Description:	This routine check to see if the current player won the game
#				on the vertical axis.
# Arguments:	a0:	current player
#				a1: the row their piece was placed in
#				a2: the column their piece was placed in
# Returns:	V0: 1 if the player has won the game, 0 otherwise.
#
check_vertical_win:
	addi	$sp, $sp, -40	# space for return address
	sw		$ra, 32($sp)	# store the ra on the stack
	sw		$s7, 28($sp)
	sw		$s6, 24($sp)
	sw		$s5, 20($sp)
	sw		$s4, 16($sp)
	sw		$s3, 12($sp)
	sw		$s2, 8($sp)
	sw		$s1, 4($sp)
	sw		$s0, 0($sp)

	la		$s0, array			# get the array
	la		$s1, MAX_COLS		# rowSize
	la		$s2, MAX_ROWS		# number of rows
	addi	$s3, $zero, 4		# win condition

	add		$t0, $zero, $a1		# a counter, starting at the selected row

	mult	$a1, $s1		# row * rowSize
	mflo	$t1
	add		$t1, $t1, $a2	# row * rowSize + col
	add		$s0, $s0, $t1	# s0 = row * rowSize + col

	move	$t2, $zero		# count consecutive pieces

vertical_loop:
	slt		$t1, $t0, $s2	# while counter < max_rows -> t1 = 1
	beq		$t1, $zero, no_vert_winner	# if t1 = 0 no winner

	lb		$t3, 0($s0)		# get the byte at the array index
	beq		$t3, $a0, vertical_positive	

	move	$t2, $zero		# reset win counter
	j		vertical_else

vertical_positive:
	addi	$t2, $t2, 1			# increment a win point

vertical_else:
	beq		$t2, $s3, vert_winner	# winner found
	addi	$s0, $s0, MAX_COLS	# go down a row
	addi	$t0, $t0, 1			# increment counter
	j		vertical_loop

vert_winner:
	addi	$v0, $zero, 1
	j		vert_finish

no_vert_winner:
	move	$v0, $zero
	j		vert_finish

vert_finish:
	lw		$ra, 32($sp)
	lw		$s7, 28($sp)
	lw		$s6, 24($sp)
	lw		$s5, 20($sp)
	lw		$s4, 16($sp)
	lw		$s3, 12($sp)
	lw		$s2, 8($sp)
	lw		$s1, 4($sp)
	lw		$s0, 0($sp)
	addi	$sp, $sp, 40	#clean up the stack
	jr	$ra


#
# Name:		check_horizontal_win
#
# Description:	This routine check to see if the current player won the game
#				on the horizontal axis.
# Arguments:	a0:	current player
#				a1: the row their piece was placed in
#				a2: the column their piece was placed in
# Returns:	V0: 1 if the player has won the game, 0 otherwise.
#
check_horizontal_win:
	addi	$sp, $sp, -40	# space for return address
	sw		$ra, 32($sp)	# store the ra on the stack
	sw		$s7, 28($sp)
	sw		$s6, 24($sp)
	sw		$s5, 20($sp)
	sw		$s4, 16($sp)
	sw		$s3, 12($sp)
	sw		$s2, 8($sp)
	sw		$s1, 4($sp)
	sw		$s0, 0($sp)

	la		$s0, array			# get the array
	la		$s1, MAX_COLS		# rowSize
	la		$s2, MAX_ROWS		# number of rows
	addi	$s3, $zero, 4		# win condition

	mult	$a1, $s1			# row * lengthOfRow
	mflo	$t1
	add		$s0, $s0, $t1		# s0 = column 0 of selected row

	move	$t0, $zero			# loop counter
	move	$t2, $zero			# number of consecutive pieces
	
horizontal_loop:
	slt		$t1, $t0, $s1		# while counter < max cols -> t1 = 1
	beq		$t1, $zero, no_horizontal_winner	# if t0 = 1 no winner

	lb		$t3, 0($s0)			# get the byte at the array index
	beq		$t3, $a0, horizontal_positive

	move	$t2, $zero			# reset win counter
	j		horizontal_else

horizontal_positive:
	addi	$t2, $t2, 1			# increment a win point

horizontal_else:
	beq		$t2, $s3, horizontal_winner		# winner found
	addi	$s0, $s0, 1			# shift right one index
	addi	$t0, $t0, 1			# increment loop counter
	j		horizontal_loop

horizontal_winner:
	addi	$v0, $zero, 1
	j		horizontal_finish

no_horizontal_winner:
	move	$v0, $zero
	j		horizontal_finish

horizontal_finish:
	lw		$ra, 32($sp)
	lw		$s7, 28($sp)
	lw		$s6, 24($sp)
	lw		$s5, 20($sp)
	lw		$s4, 16($sp)
	lw		$s3, 12($sp)
	lw		$s2, 8($sp)
	lw		$s1, 4($sp)
	lw		$s0, 0($sp)
	addi	$sp, $sp, 40	#clean up the stack
	jr	$ra

#
# Name:		check_diagonal_win
#
# Description:	This routine check to see if the current player won the game
#				on one of the diagonals.
# Arguments:	a0:	current player
#				a1: the row their piece was placed in
#				a2: the column their piece was placed in
# Returns:	V0: 1 if the player has won the game, 0 otherwise.
#
check_diagonal_win:
	addi	$sp, $sp, -40	# space for return address
	sw		$ra, 32($sp)	# store the ra on the stack
	sw		$s7, 28($sp)
	sw		$s6, 24($sp)
	sw		$s5, 20($sp)
	sw		$s4, 16($sp)
	sw		$s3, 12($sp)
	sw		$s2, 8($sp)
	sw		$s1, 4($sp)
	sw		$s0, 0($sp)

	la		$s0, array			# get the array
	la		$s1, MAX_COLS		# rowSize
	la		$s2, MAX_ROWS		# number of rows
	addi	$s3, $zero, 4		# win condition
	addi	$s4, $zero, 42		# loop stop condition

# down diagonals
	slt		$t0, $a1, $a2
	beq		$t0, $zero, down_bot_half	
# check top half of down diagonals
	
	move	$t5, $zero			# starting row
	sub		$t6, $a2, $a1		# starting column

	mult	$t5, $s1			# starting row * rowLength
	mflo	$t1
	add		$t2, $t1, $t6		# + starting column
	add		$s0, $s0, $t2		# s0 = startingRow * rowLength + startingCol

	move	$t0, $t2			# get the current index
	move	$t2, $zero			# number of consecutive pieces

down_top_half_loop:
	slt		$t1, $t0, $s4		# while index < stop condition
	beq		$t1, $zero, end_down_diagonal	# end loop

	lb		$t3, 0($s0)			# get the byte at the array index
	beq		$t3, $a0, top_half_down_positive

	move	$t2, $zero			# reset win counter
	j		top_half_down_else

top_half_down_positive:
	addi	$t2, $t2, 1			# increment the win counter

top_half_down_else:
	beq		$t2, $s3, diagonal_winner	# winner found
	addi	$s0, $s0, 8			# move to the next index
	addi	$t0, $t0, 8			# update the index counter
	j		down_top_half_loop

down_bot_half:					# check bottom half of down diagonals
	la		$s0, array
	
	sub		$t5, $a1, $a2		# starting row
	move	$t6, $zero			# starting column

	mult	$t5, $s1			# startingRow * rowLength
	mflo	$t1
	add		$t2, $t1, $t6		# + starting column
	add		$s0, $s0, $t2		# s0 = startRow * rowLength + startCol

	move	$t0, $t2			# current index
	move	$t2, $zero			# number of consecutive pieces

down_bot_half_loop:
	slt		$t1, $t0, $s4		# while index < stop condition
	beq		$t1, $zero, end_down_diagonal	# end loop

	lb		$t3, 0($s0)			# get the byte at the array index
	beq		$t3, $a0, bot_half_down_positive

	move	$t2, $zero			# reset win counter
	j		bot_half_down_else

bot_half_down_positive:
	addi	$t2, $t2, 1			# increment the win counter

bot_half_down_else:
	beq		$t2, $s3, diagonal_winner	# winner found
	addi	$s0, $s0, 8			# move to the next index
	addi	$t0, $t0, 8			# update the index counter
	j		down_bot_half_loop

end_down_diagonal:
	la		$s0, array		# get the array again

# up diagonals
	li		$t1, 5
	add		$t2, $a1, $a2
	slt		$t0, $t1, $t2	#  5 < row + col
	bne		$t0, $zero, up_bot_half
# check top half of up diagonals

	move	$t5, $t2		# starting row
	move	$t6, $zero		# starting column

	mult	$t5, $s1		# starting row * rowLength
	mflo	$t1
	add		$t2, $t1, $t6	# + starting column
	add		$s0, $s0, $t2	# s0 = startingRow * rowLength + starting col

	move	$t2, $zero		# number of consecutive pieces

up_top_half_loop:
	slt		$t1, $zero, $t5	# while row > zero
	beq		$t1, $zero, no_diagonal_winner	# no winners

	lb		$t3, 0($s0)		# get the byte at the array index
	beq		$t3, $a0, top_half_up_positive

	move	$t2, $zero		# reset win counter
	j		top_half_up_else

top_half_up_positive:
	addi	$t2, $t2, 1		# increment win counter

top_half_up_else:
	beq		$t2, $s3, diagonal_winner	# winner found
	addi	$s0, $s0, -6	# move to the next index
	addi	$t5, $t5, -1	# decrement the row
	j		up_top_half_loop

up_bot_half:
	li		$t5, 5			# starting row
	add		$t0, $a1, $a2
	sub		$t6, $t0, $t5	# starting column

	mult	$t5, $s1		# startingRow * rowLength
	mflo	$t1
	add		$t2, $t1, $t6	# + starting column
	add		$s0, $s0, $t2		# s0 = row*length + col

	move	$t0, $t6		# current column
	move	$t2, $zero		# number of consecutive pieces

up_bot_half_loop:
	slti	$t1, $t0, MAX_COLS	# while column < max_columns
	beq		$t1, $zero, no_diagonal_winner	# no winner

	lb		$t3, 0($s0)		# get the byte at the array index
	beq		$t3, $a0, bot_half_up_positive

	move	$t2, $zero		# reset win counter
	j		bot_half_up_else

bot_half_up_positive:
	addi	$t2, $t2, 1		# increment the win counter

bot_half_up_else:
	beq		$t2, $s3, diagonal_winner	# winner found
	addi	$s0, $s0, -6	# move to the next index
	addi	$t0, $t0, 1		# update the check condition
	j		up_bot_half_loop

diagonal_winner:
	addi	$v0, $zero, 1
	j		diagonal_finish

no_diagonal_winner:
	move	$v0, $zero
	j		diagonal_finish

diagonal_finish:
	lw		$ra, 32($sp)
	lw		$s7, 28($sp)
	lw		$s6, 24($sp)
	lw		$s5, 20($sp)
	lw		$s4, 16($sp)
	lw		$s3, 12($sp)
	lw		$s2, 8($sp)
	lw		$s1, 4($sp)
	lw		$s0, 0($sp)
	addi	$sp, $sp, 40	#clean up the stack
	jr	$ra


#
# Name:		check_draw
#
# Description:	This routine check to see if the game is a draw
# Arguments:	none
# Returns:	V0: 1 if the game is drawn, 0 otherwise.
#
check_draw:
	addi	$sp, $sp, -40	# space for return address
	sw		$ra, 32($sp)	# store the ra on the stack
	sw		$s7, 28($sp)
	sw		$s6, 24($sp)
	sw		$s5, 20($sp)
	sw		$s4, 16($sp)
	sw		$s3, 12($sp)
	sw		$s2, 8($sp)
	sw		$s1, 4($sp)
	sw		$s0, 0($sp)

	la		$s0, array		# get the array
	move	$t0, $zero		# a counter

draw_check:
	slti	$t1, $t0, MAX_COLS	# while counter is less than columns
	beq		$t1, $zero, game_drew

	lb		$t3, 0($s0)		# get the value
	beq		$t3, $zero, no_draw

	addi	$s0, $s0, 1		# next column
	addi	$t0, $t0, 1		# increment counter
	j		draw_check

game_drew:
	addi	$v0, $zero, 1
	j		finish_draw_check

no_draw:
	move	$v0, $zero
	j		finish_draw_check

finish_draw_check:
	lw		$ra, 32($sp)
	lw		$s7, 28($sp)
	lw		$s6, 24($sp)
	lw		$s5, 20($sp)
	lw		$s4, 16($sp)
	lw		$s3, 12($sp)
	lw		$s2, 8($sp)
	lw		$s1, 4($sp)
	lw		$s0, 0($sp)
	addi	$sp, $sp, 40	#clean up the stack
	jr	$ra
