package com.example.e_bicycle_v1;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Timer;


public class HomeFragment extends Fragment {

    public static BluetoothDevice connectDevice;
    public static BluetoothSocket connectSocket;
    public static OutputStream outputStream = null;
    public static InputStream inputStream = null;
    int counter =0;
    String speed = "";
    String soc = "";
    String distance = "";
    Button abutton;
    Button bbutton;
    TextView speedView;
    TextView socView;
    TextView disView;

    public HomeFragment(BluetoothDevice device, BluetoothSocket socket, InputStream inS, OutputStream outS){
        connectDevice = device;
        connectSocket = socket;
        outputStream = outS;
        inputStream = inS;
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_home, container, false);

        abutton = view.findViewById(R.id.button2);
        bbutton = view.findViewById(R.id.button4);
        speedView = view.findViewById(R.id.textSpeed);
        socView = view.findViewById(R.id.textSoc);
        disView = view.findViewById(R.id.textDistance);


        // Electric mode activate-------------------------------------------------------------------
        abutton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                if (connectSocket == null) {
                    Toast.makeText(getActivity(), "Please Connect to device", Toast.LENGTH_SHORT).show();
                }
                else {
                    try {
                        outputStream.write(32);
                        outputStream.write(49);
                    } catch (IOException e) {
                        Toast.makeText(getActivity(), "output " + e, Toast.LENGTH_SHORT).show();
                    }
                }
            }
        });
        // -----------------------------------------------------------------------------------------


        // Automatic mode active--------------------------------------------------------------------
        bbutton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (connectSocket == null) {
                    Toast.makeText(getActivity(), "Please Connect to device", Toast.LENGTH_SHORT).show();
                }
            }
        });
        // -----------------------------------------------------------------------------------------


        // BLU loop --------------------------------------------------------------------------------
        start();

        return view;
    }



    // BLU loop function----------------------------------------------------------------------------
    private Handler handler = new Handler();
    private Runnable runnable = new Runnable() {
        @Override
        public void run() {
            if (inputStream != null) {
                try {
                    String str1 = "";
                    inputStream.skip(inputStream.available());
                    byte b = (byte) inputStream.read();

                    if ((char) b == 47){
                        while (true) {
                            b = (byte) inputStream.read();

                            if ( (char) b == 47 ) {
                                break;
                            }
                            else {
                                str1 = str1 + (char) b;
                            }
                        }

                        String[] parts = str1.split(":");
                        speed = parts[0];
                        soc = parts[1] + "%";
                        distance = parts[2];

                        speedView.setText(speed);
                        socView.setText(soc);
                        disView.setText(distance);
                    }
                } catch (IOException e) {
                    Toast.makeText(getActivity(), "Data Reading Error", Toast.LENGTH_SHORT).show();
                }
            }
            start();
        }
    };

    public void start(){
        handler.postDelayed(runnable, 500);
    }
    // ---------------------------------------------------------------------------------------------

}