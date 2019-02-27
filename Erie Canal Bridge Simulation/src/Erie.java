/**
 * @filename Erie.java
 * @author Jacob Manelius
 * @version java version 1.8
 * @date 2/12/19
 */

import java.util.ArrayList;

/**
 * Main class, Erie houses the delay variables and main methods
 */
public class Erie
{
    private static final String usageMessage = "Usage: java Erie number-of-cars";
    //number of cars total
    private static int numberOfCars;
    //delay variables in milliseconds
    private static int approachDelay = 500;
    private static int crossingDelay = 1000;
    private static int crossingDelayMin = 500;

    //random number generator seed
    private static int randomSeed = 31;

    /**
     * Main method, creates the bridge, cars, and starts the threads.
     * @param args The number of car threads to run
     */
    public static void main(String[] args)
    {

        //region Input handling
        //check to see an argument is present
        if (args.length == 0 || args.length > 1)
        {
            System.out.println(usageMessage);
            System.exit(1);
        }

        //check to see if the argument is a number
        try
        {
            numberOfCars = Integer.parseInt(args[0]);
        } catch (NumberFormatException e)
        {
            System.out.println(usageMessage);
            System.exit(1);
        }

        //check if the argument is greater than one
        if (numberOfCars < 1)
        {
            System.out.println(usageMessage);
            System.exit(1);
        }
        //endregion

        //make the bridge
        Bridge bridge = new Bridge();

        //make all of the car threads
        ArrayList<Car> cars = new ArrayList<>();
        for (int i = 1; i <= numberOfCars; i++)
        {
            cars.add(new Car(i, approachDelay, randomSeed, crossingDelay,
                    crossingDelayMin, bridge));
        }

        //print the header
        synchronized (System.out)
        {
            System.out.println(
                    "South                        Bridge ==>     North");
        }

        //start all the cars
        for (int i = 0; i < numberOfCars; i++)
        {
            cars.get(i).start();
        }

        //join the cars
        for (int i = 0; i < numberOfCars; i++)
        {
            try
            {
                cars.get(i).join();
            } catch (InterruptedException e)
            {
                e.printStackTrace();
            }
        }

        //print end message
        synchronized (System.out)
        {
            System.out.println("simulation finished.");
        }
    }
}
