Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Industrial Digital In 4 Bricklet 2.0

    ' Callback subroutine for value callback
    Sub ValueCB(ByVal sender As BrickletIndustrialDigitalIn4V2, ByVal channel As Byte, _
                ByVal changed As Boolean, ByVal value As Boolean)
        If channel = BrickletIndustrialDigitalIn4V2.CHANNEL_0 Then
            Console.WriteLine("Channel: 0")
        Else If channel = BrickletIndustrialDigitalIn4V2.CHANNEL_1 Then
            Console.WriteLine("Channel: 1")
        Else If channel = BrickletIndustrialDigitalIn4V2.CHANNEL_2 Then
            Console.WriteLine("Channel: 2")
        Else If channel = BrickletIndustrialDigitalIn4V2.CHANNEL_3 Then
            Console.WriteLine("Channel: 3")
        End If

        Console.WriteLine("Changed: " + changed.ToString())
        Console.WriteLine("Value: " + value.ToString())
        Console.WriteLine("")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim idi4 As New BrickletIndustrialDigitalIn4V2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register value callback to subroutine ValueCB
        AddHandler idi4.ValueCallback, AddressOf ValueCB

        ' Set period for value (channel 1) callback to 0.1s (100ms)
        idi4.SetValueCallbackConfiguration(1, 100, False)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
