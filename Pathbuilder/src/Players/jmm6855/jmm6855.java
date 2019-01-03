package Players.jmm6855;

import Interface.PlayerModulePart1;
import Interface.PlayerMove;
import java.util.*;
import Interface.Coordinate;

public class jmm6855 implements PlayerModulePart1
{
    // for easy navigation of the board
    private final int BLUE = 2;
    private final int RED = 1;
    private final int EMPTY = 0;
    
    // player information
    private int size;
    private int playerId;
    private int[][] board;
    private ArrayList<Coordinate> redStartSpots;
    private ArrayList<Coordinate> redEndSpots;
    private ArrayList<Coordinate> blueStartSpots;
    private ArrayList<Coordinate> blueEndSpots;
    private boolean firstMove = false;
    
    // set up internal structure for the board
    @Override
    public void initPlayer(int dim, int playerId)
    {
        redStartSpots = new ArrayList<Coordinate>();
        redEndSpots = new ArrayList<Coordinate>();
        blueStartSpots = new ArrayList<Coordinate>();
        blueEndSpots = new ArrayList<Coordinate>();
        this.size = 2 * dim + 1;
        this.playerId = playerId;
        this.board = new int[this.size][this.size];
        resetBoard();
        
        for (int row = 1; row < size; row += 2)
        {
            redStartSpots.add(new Coordinate(row, 0));
            redEndSpots.add(new Coordinate(row, size - 1));
        }
        for (int col = 1; col < size; col += 2)
        {
            blueStartSpots.add(new Coordinate(0, col));
            blueEndSpots.add(new Coordinate(size - 1, col));
        }
    }
    
    private List<PlayerMove> allLegalMoves(int[][] board)
    {
        List<PlayerMove> moves = new ArrayList<PlayerMove>();
        for (int row = 0; row < size; row++)
        {
            for (int column = 0; column < size; column++)
            {
                if (row != 0 && row != size - 1 && column != 0 && column != size - 1)
                {
                    if (board[row][column] == 0)
                        moves.add(new PlayerMove(new Coordinate(row, column), this.playerId));
                }
            }
        }
        return moves;
    }
    
    // resets the board to the starting values
    public void resetBoard()
    {
        for (int row = 0; row < size; row++)
        {
            for (int column = 0; column < size; column++)
            {
                this.board[row][column] = 0; // just in case java thinks empty
                                             // arrays are null
                if (row % 2 == 0)
                {
                    if (column % 2 == 1)
                        this.board[row][column] = BLUE;
                    else
                        this.board[row][column] = EMPTY;
                }
                else
                {
                    if (column % 2 == 1)
                        this.board[row][column] = EMPTY;
                    else
                        this.board[row][column] = RED;
                }
            }
        }
    }
    
    // updates the internal board structure
    @Override
    public void lastMove(PlayerMove m)
    {
        int player = m.getPlayerId();
        int row = m.getCoordinate().getRow();
        int col = m.getCoordinate().getCol();
        this.board[row][col] = player;
        firstMove = true;
    }
    
    // decide how to make the next move
    @Override
    public PlayerMove move()
    {
        Coordinate myMove = null;
        if (playerId == RED)// has a special case for first move
        {
            if (!firstMove) // first move case
            {
                boolean flag = false;
                while (!flag)
                {
                    Coordinate firstMove = redStartSpots.get((int) (Math.random() * (size/2-1)));
                    myMove = new Coordinate(firstMove.getRow(), firstMove.getCol() + 1);
                    if (this.board[myMove.getRow()][myMove.getCol()] == 0)
                        flag = true;
                }
                firstMove = true;
                return new PlayerMove(myMove, this.playerId);
            }
            
            // deciding a move
            List<PlayerMove> moves = allLegalMoves(this.board);
            return moves.get(0);
        }
        else
        {
            int[][] rotatedBoard = new int[size][size];
            for (int row = 0; row < size; row++)
            {
                for (int col = 0; col < size; col++)
                {
                    rotatedBoard[row][col] = this.board[size - col - 1][row];
                }
            }
            List<PlayerMove> blueMoves = allLegalMoves(rotatedBoard);
            return blueMoves.get(0);
        }
    }
    
    @Override
    // what is this method supposed to do?
    public void otherPlayerInvalidated()
    {
        move();
    }
    
    // checks if a player has won the game
    @Override
    public boolean hasWonGame(int id)
    {
        boolean solved = false;
        // track visited sections
        ArrayList<Coordinate> redVisited = new ArrayList();
        ArrayList<Coordinate> blueVisited = new ArrayList();
        if (id == RED)
        {
            for (Coordinate c : redStartSpots)
                redVisited.add(c);
            for (Coordinate c : redStartSpots)
            {
                solved = solve(c, this.board, redVisited, id);
                if (solved)
                    return true;
            }
        }
        else
        {
            for (Coordinate c : blueStartSpots)
                blueVisited.add(c);
            for (Coordinate c : blueStartSpots)
            {
                solved = solve(c, this.board, blueVisited, id);
                if (solved)
                    return true;
            }
        }
        return false;
    }
    
    private boolean solve(Coordinate c, int[][] board, ArrayList<Coordinate> visited, int id)
    {
        boolean solved = false;// not solved yet
        visited.add(c);// I've been here
        // current row and column
        int row = c.getRow();
        int col = c.getCol();
        
        int right = -1;
        int left = -1;
        int up = -1;
        int down = -1;
        
        Coordinate rightCord = null;
        Coordinate leftCord = null;
        Coordinate upCord = null;
        Coordinate downCord = null;
        
        // bounds checks
        if (col + 1 <= size - 1)
        {
            right = board[row][col + 1];
            rightCord = new Coordinate(row, col + 1);
        }
        if (col - 1 >= 0)
        {
            left = board[row][col - 1];
            leftCord = new Coordinate(row, col - 1);
        }
        if (row - 1 >= 0)
        {
            up = board[row - 1][col];
            upCord = new Coordinate(row - 1, col);
        }
        if (row + 1 <= size - 1)
        {
            down = board[row + 1][col];
            downCord = new Coordinate(row + 1, col);
        }
        
        // you won the game, good job
        if (redEndSpots.contains(rightCord) || redEndSpots.contains(leftCord) || redEndSpots.contains(upCord)
                || redEndSpots.contains(downCord))
            return true;
            
        if (right == id && !visited.contains(rightCord))
            solved = solve(rightCord, board, visited, id);
        if (left == id && !visited.contains(leftCord) && !solved)
            solved = solve(leftCord, board, visited, id);
        if (up == id && !visited.contains(upCord) && !solved)
            solved = solve(upCord, board, visited, id);
        if (down == id && !visited.contains(downCord) && !solved)
            solved = solve(downCord, board, visited, id);
            
        return solved;
    }
    
}
