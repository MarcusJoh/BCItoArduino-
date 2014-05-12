


// a part form ThinkUino Project application 
// http://www.codeproject.com/Articles/567963/ThinkUino-Project 





using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using ThinkGearNET;
using System.Threading;
using System.IO;

namespace ThinkUino
{
    public partial class Form1 : Form
    {
        
        private ThinkGearWrapper thinkGearWrapper = new ThinkGearWrapper(); //declare the thinkgear object
        private bool comando = false;

        private const string ARDUPORT = "COM4"; //declare the arduino serial port
        private const string MINDPORT = "COM25"; //declare the Mindwave serial port

        public Form1()
        {
           InitializeComponent();
        }

        private void btnHeadsetConnect_Click(object sender, EventArgs e)
        {
            if (thinkGearWrapper.Connect(MINDPORT, 57600, true)) //check if the Mindwave is connected
            {
                btnHeadsetConnect.Enabled = false; //disable connect button
                btnHeadsetDisconnect.Enabled = true; //enable disconnect button
                txtHeadsetDebug.AppendText("Headset connected." + Environment.NewLine);
                thinkGearWrapper.EnableBlinkDetection(true); //enable the eye blink on the eSense protocol
                thinkGearWrapper.ThinkGearChanged += new EventHandler<ThinkGearChangedEventArgs>(thinkGearWrapper_ThinkGearChanged); //capture the event when a new data is incoming
            }
            else
                txtHeadsetDebug.AppendText("Could not connect to headset" + Environment.NewLine);
        }

        void thinkGearWrapper_ThinkGearChanged(object sender, ThinkGearChangedEventArgs e)
        {
            
            BeginInvoke(new MethodInvoker(delegate //use the AsyncTask to update the UI
                { 
								
						// Code modified for the "Accessibility through the brain" project 
				//-------------------------------------------------------------------------------------------------------------------------------------------------------
                    if ((thinkGearWrapper.ThinkGearState.BlinkStrength > 60)  ) // check if the eye blink strength is above 60
                    {
                        comando = !comando;
                        txtHeadsetDebug.AppendText("Command1: " + Convert.ToByte(comando) + Environment.NewLine); 
                        thinkGearWrapper.ThinkGearState.BlinkStrength = 0;//set the eye blink strength to zero
                    }
                    if (thinkGearWrapper.ThinkGearState.Attention > 10)//check if the Attention state is above 60
                    {
                       
                        if (comando == true) //Check if the user have blinked or not
                        {
                            float kiloval =100; //adds on the focus value to let the arduino know that the use have blinked
                            float comval = thinkGearWrapper.ThinkGearState.Attention + kiloval;
                            byte val = Convert.ToByte(comval);
                            serialArduino.Write(new byte[] { val }, 0, 1); //send the commando to arduino serial port 

                            txtHeadsetDebug.AppendText("Command sendend: " + Convert.ToByte(comval) + Environment.NewLine);
                            txtArduinoDebug.AppendText("Command received: " + Convert.ToByte(comval) + Environment.NewLine);
                            thinkGearWrapper.ThinkGearState.Attention = 0; //set the attention state to zero
                        }
                        else {
                            serialArduino.Write(new byte[] { Convert.ToByte(thinkGearWrapper.ThinkGearState.Attention) }, 0, 1); //send the commando to arduino serial port 

                        txtHeadsetDebug.AppendText("Command sendend: " + Convert.ToByte(thinkGearWrapper.ThinkGearState.Attention)  + Environment.NewLine);
                        txtArduinoDebug.AppendText("Command received: " + Convert.ToByte(thinkGearWrapper.ThinkGearState.Attention) + Environment.NewLine);
                        thinkGearWrapper.ThinkGearState.Attention = 0; //set the attention state to zero 
                       } 
					//-------------------------------------------------------------------------------------------------------------------------------------------------------
                    }
                    lblAttention.Text = "Attention: " + thinkGearWrapper.ThinkGearState.Attention;
                    lblMeditation.Text = "Meditation: " + thinkGearWrapper.ThinkGearState.Meditation;
                }));
            Thread.Sleep(10); //wait ten milliseconds
        }

        private void btnHeadsetDisconnect_Click(object sender, EventArgs e)
        {
            
            thinkGearWrapper.Disconnect(); //close the headset connection
            txtHeadsetDebug.AppendText("Disconnected." + Environment.NewLine);
            btnHeadsetDisconnect.Enabled = false; //disable the headset disconnect button
            btnHeadsetConnect.Enabled = true; //enable the headset connect button
        }

        private void bntArduinoConnect_Click(object sender, EventArgs e)
        {
            try
            {
                serialArduino.PortName = ARDUPORT; //set the arduino serial port name
                serialArduino.Open(); //open the communication
                txtArduinoDebug.AppendText("Arduino connected" + Environment.NewLine);
                btnArduinoDisconnect.Enabled = true; //enable the arduino disconnect button
                btnArduinoConnect.Enabled = false; //disable the arduino connect button
            }
            catch (IOException)
            {
                txtArduinoDebug.AppendText("Could not connect to Arduino" + Environment.NewLine);
            }
               
        }

        private void btnArduinoDisconnect_Click(object sender, EventArgs e)
        {
            if (serialArduino.IsOpen) //check if the communication is open
            {
                serialArduino.Close(); //close the arduino communication
                txtArduinoDebug.AppendText("Disconnected." + Environment.NewLine);
                btnArduinoConnect.Enabled = true; //enable the arduino connect button
                btnArduinoDisconnect.Enabled = false; //disable the arduino disconnect button
            }
        }

    }
}
