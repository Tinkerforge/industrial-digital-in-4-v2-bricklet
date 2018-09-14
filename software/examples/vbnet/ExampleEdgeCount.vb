Imports System
Imports System.Threading
Imports Tinkerforge

Module ExampleEdgeCount
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim idi4 As New BrickletIndustrialDigitalIn4V2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Configure rising edge count (channel 3) with 10ms debounce
        idi4.SetEdgeCountConfiguration(3, 0, 10)

        ' Get edge count 10 times with 1s delay
        Dim i As Integer
        For i = 0 To 9
            Thread.Sleep(1000)

            ' Get current edge count
            Dim count As Long = idi4.GetEdgeCount(3, False)
            Console.WriteLine("Count: " + count.ToString())
        Next i

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
