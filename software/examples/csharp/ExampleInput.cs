using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIndustrialDigitalIn4V2 idi4 =
		  new BrickletIndustrialDigitalIn4V2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current value
		bool value = idi4.GetValue();

		Console.WriteLine("Channel 0: " + value[0]);
		Console.WriteLine("Channel 1: " + value[1]);
		Console.WriteLine("Channel 2: " + value[2]);
		Console.WriteLine("Channel 3: " + value[3]);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
