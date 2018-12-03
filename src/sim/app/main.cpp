// From FW application main
extern void setup();
extern void loop();

// From simulated HW definition
extern void setup_app_hw_sim();

int main(int argc, char *argv[])
{
    /// @todo Initialize the simulated MPU.
    /// @todo Start clock for millis, etc...

    // Allow the simulation to setup its HW
    setup_app_hw_sim();

    // Run the FW.
    setup();
    for(;;)
    {
        loop();
    }

    return 0;
}