package pl.c0.kaczuch.pw;

import java.util.Random;


class Thr extends Thread
{
    private final int th_number;
    private final int nr_iterations;
    private final int v1[];
    private final int v2[];
    private int v3[];

    public Thr(int th_number, int nr_iterations, int v1[], int v2[], int v3[])
    {
        this.th_number = th_number;
        this.nr_iterations = nr_iterations;
        this.v1 = v1;
        this.v2 = v2;
        this.v3 = v3;
    }

    public void run()
    {
        for (int i = nr_iterations * th_number; i < nr_iterations * (th_number + 1); i++)
            v3[i] = v1[i] + v2[i];
    }

    public int getTh_number()
    {
        return th_number;
    }

    public int getNr_iterations()
    {
        return nr_iterations;
    }

    public int[] getV1()
    {
        return v1;
    }

    public int[] getV2()
    {
        return v2;
    }

    public int[] getV3()
    {
        return v3;
    }

}

/**
 * additional thread
 * if remainder is set
 */
class ThrRemider extends Thr
{
    private int rest;

    public ThrRemider(int th_number, int nr_iterations, int[] v1, int[] v2, int[] v3, int rest)
    {
        super(th_number, nr_iterations, v1, v2, v3);
        this.rest = rest;
    }

    public void run()
    {
        int temp = getTh_number() * getNr_iterations();
        for (int i = temp; i < temp + this.rest; i++)
        {
            getV3()[i] = getV1()[i] + getV2()[i];
        }
    }
}

public class Zad1
{
    private static final Random random = new Random();

    public static int[] create_vector(int n)
    {
        int[] v = new int[n];

        for (int i = 0; i < n; i++)
            v[i] = random.nextInt(10);

        return v;
    }

    public static void main(String args[])
    {
        int num_threads = Runtime.getRuntime().availableProcessors();
        int i;
        int n = 10;

        int remainder = n % num_threads;

        int[] v1 = create_vector(n);
        int[] v2 = create_vector(n);
        int[] v3 = new int[n];

        int nr_iterations = n / num_threads;
        if (nr_iterations == 0)
        {
            nr_iterations = 1;
            num_threads = n;
        }

        Thr[] NewThr = new Thr[num_threads];
        ThrRemider tR = new ThrRemider(num_threads, nr_iterations, v1, v2, v3, remainder);

        for (i = 0; i < num_threads; i++)
        {
            NewThr[i] = new Thr(i, nr_iterations, v1, v2, v3);
            NewThr[i].start();
        }

        if (remainder != 0 && num_threads < n) tR.start();

        for (i = 0; i < num_threads; i++)
        {
            try
            {
                NewThr[i].join();
            }
            catch (InterruptedException e)
            {
                System.out.println(e);
            }
        }
        if (remainder != 0 && num_threads < n)
        {
            try
            {
                tR.join();
            }
            catch (InterruptedException e)
            {
                System.out.println(e);
            }
        }

        for (int j = 0; j < n; j++)
        {
            System.out.printf("%2d. %d + %d = %d\n",j,v1[j],v2[j],v3[j]);
        }
    }
}
