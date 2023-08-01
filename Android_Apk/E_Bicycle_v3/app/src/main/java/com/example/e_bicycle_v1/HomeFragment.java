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

        if (MainActivity.elec_mode ==0) {
            abutton.setText("electric mode : off");
        }
        else {
            abutton.setText("electric mode : on");
        }



        // Electric mode activate-------------------------------------------------------------------
        abutton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                if (connectSocket == null) {
                    Toast.makeText(getActivity(), "Please Connect to device", Toast.LENGTH_SHORT).show();
                }
                else {
                    if (MainActivity.elec_mode==0) {
                        MainActivity.elec_mode = 1;
                        E_Start();
                        abutton.setText("electric mode : on");

                    } else if (MainActivity.elec_mode==1) {
                        MainActivity.elec_mode =0;
                        E_Stop();
                        abutton.setText("electric mode : off");
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
                    startActivity(new Intent(getActivity() , OpenCvCam.class).addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP));

                }
                else {
                    if (MainActivity.elec_mode == 0) {
                        MainActivity.auto_mode = 1;
                        startActivity(new Intent(getActivity(), OpenCvCam.class).addFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP));
//                        A_Start();
                    }
                    else {
                        Toast.makeText(getActivity(), "Please Turn off Electric Mode", Toast.LENGTH_SHORT).show();
                    }
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
        handler.postDelayed(runnable, 200);
    }
    // ---------------------------------------------------------------------------------------------


    public void E_Stop(){
        try {
            outputStream.write(70);
            MainActivity.elec_mode = 0;
            outputStream.write(70);
        } catch (IOException e) {
            Toast.makeText(getActivity(), "E_stop er " + e, Toast.LENGTH_SHORT).show();
        }
    }

    public void E_Start(){
        try {
            outputStream.write(69);
            MainActivity.elec_mode = 1;
            outputStream.write(69);
        } catch (IOException e) {
            Toast.makeText(getActivity(), "E_start er " + e, Toast.LENGTH_SHORT).show();
        }
    }

    public void A_Start(){
        try {
            outputStream.write(65);
            MainActivity.auto_mode = 1;
            outputStream.write(65);
        } catch (IOException e) {
            Toast.makeText(getActivity(), "A_start er " + e, Toast.LENGTH_SHORT).show();
        }
    }

    public void A_Stop(){
        try {
            outputStream.write(67);
            MainActivity.auto_mode = 0;
            outputStream.write(67);
        } catch (IOException e) {
            Toast.makeText(getActivity(), "A_stop er " + e, Toast.LENGTH_SHORT).show();
        }
    }

    public void S_Stop(){
        try {
            outputStream.write(83);
            outputStream.write(83);
        } catch (IOException e) {
            Toast.makeText(getActivity(), "S_stop er " + e, Toast.LENGTH_SHORT).show();
        }
    }

    public void R_Run(){
        try {
            outputStream.write(82);
            outputStream.write(82);
        } catch (IOException e) {
            Toast.makeText(getActivity(), "R_run er " + e, Toast.LENGTH_SHORT).show();
        }
    }



    private final BroadcastReceiver broadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();

            if (BluetoothDevice.ACTION_ACL_DISCONNECTED.equals(action)){
                Toast.makeText(getActivity(), "BLU disconnected ", Toast.LENGTH_SHORT).show();
            }

        }
    };

}