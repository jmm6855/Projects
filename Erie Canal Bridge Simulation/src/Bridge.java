/**
 * @filename Bridge.java
 * @author Jacob Manelius
 * @version java version 1.8
 * @date 2/12/19
 */

import java.util.LinkedList;
import java.util.Queue;

/**
 * The monitor class for the threads
 * Houses synchronized methods to control the threads actions.
 */
public class Bridge
{
    //cars waiting from either direction
    private Queue<Integer> fromNorth;
    private Queue<Integer> fromSouth;
    //cars on the bridge
    private Queue<Integer> bridgeOccupants;
    //tracks the flow of traffic
    private TrafficFlow flow;
    //define which side is next
    private boolean goingNorthNext;

    /**
     * Create a new bridge object and set all default values
     */
    public Bridge()
    {
        flow = TrafficFlow.NORTH;
        goingNorthNext = true;
        fromNorth = new LinkedList<>();
        fromSouth = new LinkedList<>();
        bridgeOccupants = new LinkedList<>();
    }

    /**
     * A car calling this method will join the line of other cars
     * before attempting to cross the bridge. If the car should
     * not cross the bridge it will wait until it is notified.
     * @param id The id of the thread that called the method
     */
    public synchronized void crossToNorth(int id)
    {
        //add the id to the queue
        fromSouth.add(id);

        //check to see if the car gets added to the bridge
        while (!bridgeOccupants.contains(id))
        {
            //if the bridge is empty the current cars direction is set
            if (bridgeOccupants.isEmpty())
            {
                flow = TrafficFlow.NORTH;
            }

            //if no other cars are waiting, current car is next
            if (fromNorth.isEmpty())
            {
                goingNorthNext = true;
            }


            if (flow == TrafficFlow.NORTH           //traffic is moving the same direction
                    && bridgeOccupants.size() < 3   //the bridge has space
                    && goingNorthNext               //there are no cars waiting on the other side
                    && fromSouth.peek() == id)      //the current car is first in line
            {
                bridgeOccupants.add(id);
                fromSouth.remove();
                goingNorthNext = false;
                printUpdate();
            }
            else
            {
                //the current car does not enter the bridge
                //it must wait until it can
                try
                {
                    this.wait();
                } catch (InterruptedException e)
                {
                    e.printStackTrace();
                }
            }
        }
    }

    /**
     * A car calling this method will join the line of other cars
     * before attempting to cross the bridge. If the car should
     * not cross the bridge it will wait until it is notified.
     * @param id The id of the thread that called the method
     */
    public synchronized void crossToSouth(int id)
    {
        //add the id to the queue
        fromNorth.add(id);

        //check to see if the car gets added to the bridge
        while (!bridgeOccupants.contains(id))
        {
            //if the bridge is empty the current cars direction is et
            if (bridgeOccupants.isEmpty())
            {
                flow = TrafficFlow.SOUTH;
            }

            //if no cars are waiting, current car is next
            if (fromSouth.isEmpty())
            {
                goingNorthNext = false;
            }


            if (flow == TrafficFlow.SOUTH           //traffic is moving the same direction
                    && bridgeOccupants.size() < 3   //the bridge has space
                    && !goingNorthNext              //there are no cars waiting on the other side
                    && fromNorth.peek() == id)      //the current car is first in line
            {
                bridgeOccupants.add(id);
                fromNorth.remove();
                goingNorthNext = true;
                printUpdate();
            }
            else
            {
                //the current car does not enter the bridge
                //it must wait until it can
                try
                {
                    this.wait();
                } catch (InterruptedException e)
                {
                    e.printStackTrace();
                }
            }
        }
    }

    /**
     * A car calling this method is trying to exit the bridge,
     * if it is not the first car on the bridge it will need
     * to wait until it is.
     * @param id The id of the thread that called the method
     */
    public synchronized void reachedTheNorth(int id)
    {
        while (bridgeOccupants.contains(id))
        {
            //if this car was not the first on the bridge,
            //it waits until it is
            if (bridgeOccupants.peek() != id)
            {
                try
                {
                    this.wait();
                } catch (InterruptedException e)
                {
                    e.printStackTrace();
                }
            }
            else //this car is the first in line on the bridge
            {
                //remove the car from the bridge
                bridgeOccupants.remove();
                //the next car comes from the other direction
                goingNorthNext = false;
                printUpdate();
                //if the bridge is now empty, switch the active direction
                if (bridgeOccupants.size() == 0)
                {
                    flow = TrafficFlow.SOUTH;
                }
                notifyAll();
            }
        }
    }

    /**
     * A car calling this method is trying to exit the bridge,
     * if it is not the first car on the bridge it will need
     * to wait until it is.
     * @param id The id of the thread that called the method
     */
    public synchronized void reachedTheSouth(int id)
    {
        while (bridgeOccupants.contains(id))
        {
            //if this car was not the first on the bridge,
            //it waits until it is
            if (bridgeOccupants.peek() != id)
            {
                try
                {
                    this.wait();
                } catch (InterruptedException e)
                {
                    e.printStackTrace();
                }
            }
            else //this car is the first in line on the bridge
            {
                //remove the car from the bridge
                bridgeOccupants.remove();
                //the next car comes from the other direction
                goingNorthNext = true;
                printUpdate();
                //if the bridge is now empty, switch the active direction
                if (bridgeOccupants.size() == 0)
                {
                    flow = TrafficFlow.NORTH;
                }
                notifyAll();
            }
        }
    }

    /**
     * Prints the individual lines of output to System.out
     * Informs the user of the process behind the thread execution.
     */
    private synchronized void printUpdate()
    {
        //region south output string generation
        //get a string representation of the south side of cars
        //get the integers in line
        Object[] carsInSouthLine = fromSouth.toArray();
        String southLine = "[";
        if (fromSouth.isEmpty())
        {
            southLine = southLine.concat("]");
        }
        else
        {
            for (int i = carsInSouthLine.length - 1; i >= 0; i--)
            {
                southLine = southLine.concat(String.valueOf(carsInSouthLine[i]));
                if (i == 0)
                {
                    southLine = southLine.concat("]");
                }
                else
                {
                    southLine = southLine.concat(", ");
                }
            }
        }
        //endregion

        //region bridge string generation
        String bridgeLine = "";
        //get the direction of the arrow
        if (flow == TrafficFlow.NORTH)
            bridgeLine = bridgeLine.concat("==> ");
        else
            bridgeLine = bridgeLine.concat("<== ");

        //add the cars in the bridge
        if (bridgeOccupants.isEmpty())
        {
            bridgeLine = bridgeLine.concat("           ");
        }
        else
        {
            Object[] carsOnBridge = bridgeOccupants.toArray();
            for (int i = carsOnBridge.length - 1; i >= 0; i--)
            {
                bridgeLine = bridgeLine.concat(String.valueOf(carsOnBridge[i]));
                bridgeLine = bridgeLine.concat(" ");
            }
        }
        //endregion

        //region north output string generation
        //get a string representation of the cars in the line to the north
        String northLine = fromNorth.toString();
        //endregion

        //region Print statements
        //print out the update line
        synchronized (System.out)
        {
            //print the south car line with no clipping
            if (southLine.length() < 23)
            {
                System.out.print("S: " + southLine);
                for (int i = southLine.length(); i < 23; i++)
                    System.out.print(" ");
                System.out.print("   ");
            }
            else //the car line needs to be clipped
            {
                System.out.print("S: ");
                for (int i = southLine.length() - 23; i < southLine.length(); i++)
                    System.out.print(southLine.charAt(i));
                System.out.print("   ");
            }

            //print the bridge part of the update line
            System.out.print(bridgeLine);
            for (int i = bridgeLine.length(); i < 15; i++)
                System.out.print(" ");

            //print the north car line
            if (northLine.length() < 28) //print without clipping
            {
                System.out.println(northLine + " :N");
            }
            else //print with clipping
            {
                for (int i = 0; i < 28; i++)
                    System.out.print(northLine.charAt(i));
                System.out.println(" :N");
            }
        }
        //endregion
    }

    /**
     * This enum keeps track of the current direction that the traffic
     * is flowing. Used to know if a car can attempt to cross the bridge.
     */
    public enum TrafficFlow
    {
        NORTH,
        SOUTH
    }
}

