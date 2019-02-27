/**
 * @filename Car.java
 * @author Jacob Manelius
 * @version java version 1.8
 * @date 2/12/19
 */

import java.util.Random;

/**
 * Cars are the threads of the program, each one has an id and direction
 * they try to cross a bridge with one lane by using the bridge monitor class
 */
public class Car extends Thread
{
    //id for the car
    private int id;
    private Bridge bridge;
    private int approachDelay;
    private int randomSeed;
    private int crossingDelay;
    private int crossingDelayMin;

    /**
     * Create a new Car object
     * @param id The id of the new car object
     * @param approachDelay Upper bound in milliseconds for the car to approach the bridge
     * @param randomSeed Seed for the random number generator
     * @param crossingDelay Upper bound in milliseconds for the car to cross the bridge
     * @param crossingDelayMin Lower bound in milliseconds for the car to cross the bridge
     * @param bridge The bridge object, functions as a monitor
     */
    public Car(int id, int approachDelay, int randomSeed,
               int crossingDelay, int crossingDelayMin, Bridge bridge)
    {
        this.id = id;
        this.approachDelay = approachDelay;
        this.randomSeed = randomSeed;
        this.crossingDelay = crossingDelay;
        this.crossingDelayMin = crossingDelayMin;
        this.bridge = bridge;

        //print creation string
        synchronized (System.out)
        {
            System.out.println("Creating Car Driver " + id + ".");
        }
    }

    /**
     * The thread execution, defines what a car tries to do.
     * It approaches the bridge after a short time, tries to cross,
     * takes time to cross, then exits the bridge.
     */
    public void run()
    {
        //sleep thread for a random time, this is the approach delay
        Random rand = new Random(randomSeed);
        try
        {
            Thread.sleep(rand.nextInt(approachDelay));
        } catch (InterruptedException e)
        {
            e.printStackTrace();
        }

        //approach the bridge
        if (id % 2 == 0) //approach from the north
        {
            bridge.crossToSouth(id); //add the car to the line
            crossingSleep(rand); //cross the bridge
            bridge.reachedTheSouth(id); //exit the bridge
        }
        else //approach from the south
        {
            bridge.crossToNorth(id); //add the car to the line
            crossingSleep(rand); //cross the bridge
            bridge.reachedTheNorth(id); //exit the bridge
        }
    }

    /**
     * Sleeps the car thread for a random amount of time
     * @param rand
     */
    private void crossingSleep(Random rand)
    {
        int sleepTime = rand.nextInt(crossingDelay) + crossingDelayMin;
        try
        {
            Thread.sleep(sleepTime);
        } catch (InterruptedException e)
        {
            e.printStackTrace();
        }
    }
}