package pl.c0.kaczuch.pw;

import java.util.Random;

public class Zad2
{
    private static Random r = new Random();

    public static char[][] gen_char_table(int n, int m, String dic)
    {
        char[][] t = new char[n][m];
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                t[i][j] = dic.charAt(r.nextInt(dic.length()));
            }
        }
        return t;
    }

    /**
     * @param start ascii start
     * @param n     dictionary length
     * @return dictionary
     */
    public static String create_dictionary(int start, int n)
    {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < n; i++)
        {
            sb.append((char) (i + start + ' '));
        }
        return sb.toString();
    }

    public static void print_table(char[][] t)
    {
        for (char[] x : t)
        {
            for (char c : x)
            {
                System.out.print("|" + c + "| ");
            }
            System.out.println();
        }
    }



    public static void main(String[] args)
    {
        int nr_threads = Runtime.getRuntime().availableProcessors();
        int dic_lenght = 9;
        if (dic_lenght < nr_threads)
            nr_threads = dic_lenght;

        String dic = create_dictionary(33, dic_lenght);
        char[][] img = gen_char_table(100, 100, dic);

        int th_dic_lenght = dic_lenght / nr_threads;
        int remainder = dic_lenght % nr_threads;

        char[][] th_dic = new char[nr_threads][th_dic_lenght];

        for (int i = 0; i < nr_threads; i++)
        {
            th_dic[i] = dic.substring(i * th_dic_lenght, i * th_dic_lenght + th_dic_lenght).toCharArray();
            System.out.println();
        }
        char[] th_dic_reminder = dic.substring(nr_threads * remainder, nr_threads * remainder + remainder).toCharArray();

        CounterThread[] ctTable = new CounterThread[nr_threads];

        for (int i = 0; i < nr_threads; i++)
        {
            ctTable[i] = new CounterThread(img, th_dic[i]);
            ctTable[i].start();
        }
        CounterThread th_Reminder = new  CounterThread(img, th_dic_reminder);
        th_Reminder.start();

        for (int i = 0; i < nr_threads; i++)
        {
            try
            {
                ctTable[i].join();
            }
            catch (InterruptedException e)
            {
                e.printStackTrace();
            }
            System.out.println("watek: " + i);
            ctTable[i].print_res();
        }
        try
        {
            System.out.println("reszta");
            th_Reminder.join();
            th_Reminder.print_res();

        }
        catch (InterruptedException e)
        {
            e.printStackTrace();
        }
    }
}

class CounterThread extends Thread
{
    private char[] th_dic;
    private char[][] img;
    private int[] counter_th_dic;

    public CounterThread(char[][] img, char[] th_dic)
    {
        this.img = img;
        this.th_dic = th_dic;
        this.counter_th_dic = new int[th_dic.length];
    }

    public void run()
    {
        for (int i = 0; i < this.img.length; i++)
        {
            for (int j = 0; j < this.img[0].length; j++)
            {
                for (int k = 0; k < this.th_dic.length; k++)
                {
                    if (this.img[i][j] == this.th_dic[k])
                    {
                        this.counter_th_dic[k]++;
                    }
                }
            }
        }
    }

    public void print_res()
    {
        for (int i = 0; i < counter_th_dic.length; i++)
        {
            System.out.println("\t" + th_dic[i] + ": " + counter_th_dic[i]);
        }
    }
}