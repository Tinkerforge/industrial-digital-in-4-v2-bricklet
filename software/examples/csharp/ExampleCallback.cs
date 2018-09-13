using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

	// Callback function for value callback
	static void ValueCB(BrickletIndustrialDigitalIn4V2 sender, byte channel, bool changed,
	                    byte value)
	{
		Console.WriteLine("Channel: " + channel);
		Console.WriteLine("Changed: " + changed);
		Console.WriteLine("Value: " + value);
		Console.WriteLine("");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIndustrialDigitalIn4V2 idi4 =
		  new BrickletIndustrialDigitalIn4V2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register value callback to function ValueCB
		idi4.ValueCallback += ValueCB;

		// Configure callback for channel 1 with fixed 100ms period
		idi4.SetValueCallbackConfiguration(1, 100, false);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
