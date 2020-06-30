package pl.c0.kaczuch.pw;

public class Test
{
    public static void main(String[] args)
    {
        new Thread(new A()).start();
    }
}
class A implements Runnable
{
    @Override
    public void run()
    {
        System.out.println("Jestem " +  A.class.getName());
    }
}


