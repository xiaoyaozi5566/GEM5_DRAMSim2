class ExitCounter{
    public:
        static int get();
        static int dec();
    private:
        // If fastforwarding is turned of the count
        // needs to be manually changed to 0 here.
        ExitCounter(){ count=1; };
        static ExitCounter * instance;
        int count;

};
