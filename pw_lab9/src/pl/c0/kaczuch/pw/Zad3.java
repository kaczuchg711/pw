package pl.c0.kaczuch.pw;

import java.util.ArrayList;
import java.util.List;

public class Zad3
{
    public static int[] get_naturalnrs(int end)
    {
        int[] tab = new int[end];
        for (int i = 0; i < end; i++)
        {
            tab[i] = i;
        }

        return tab;
    }


    public static void main(String[] args)
    {
        int nr_thread = Runtime.getRuntime().availableProcessors();
        int n = 100;

        if(nr_thread > n)
            nr_thread = n;
        int range_length = n / nr_thread;
        int remainder = n % nr_thread;

        ArrayList<Integer> arr = new ArrayList<Integer>(n);

        int[] tab = get_naturalnrs(n);
        for (Integer a : tab)
        {
            arr.add(a);
        }

        ErastotenesThreat[] et_table = new ErastotenesThreat[nr_thread];
        ErastotenesThreat et_remainder;

        for (int i = 2; i < n; i++)
        {
            if (arr.get(i) != 0)
                ErastotenesThreat.setDEL(arr.get(i));

            for (int j = 0; j < nr_thread; j++)
            {
                et_table[j] = new ErastotenesThreat(arr.subList(j * range_length, j * range_length + range_length));
                et_table[j].start();
            }
            et_remainder = new ErastotenesThreat(arr.subList(nr_thread * range_length,nr_thread * range_length + remainder));
            et_remainder.start();

            for (int j = 0; j < nr_thread; j++)
            {
                try
                {
                    et_table[j].join();
                }
                catch (InterruptedException e)
                {
                    e.printStackTrace();
                }
            }
            try
            {
                et_remainder.join();
            }
            catch (InterruptedException e)
            {
                e.printStackTrace();
            }
        }
        System.out.println(arr);
    }
}

class ErastotenesThreat extends Thread
{
    private static int DEL;
    private List<Integer> arr;

    public ErastotenesThreat(List<Integer> arr)
    {
        this.arr = arr;
    }

    public static void setDEL(int DEL)
    {
        ErastotenesThreat.DEL = DEL;
    }

    @Override
    public void run()
    {
        for (int i = 0; i < arr.size(); i++)
        {
            if ((arr.get(i) % DEL == 0) && arr.get(i) != DEL)
            {
                arr.set(i, 0);
            }
        }
    }
}
