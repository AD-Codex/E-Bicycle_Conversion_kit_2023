package com.example.e_bicycle_v1;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.Toast;

import java.io.IOException;
import java.io.OutputStream;


public class HomeFragment extends Fragment {

    public static BluetoothDevice connectDevice;
    public static BluetoothSocket connectSocket;

    Button abutton;
    String str = "Home";

    public HomeFragment(String text, BluetoothDevice device, BluetoothSocket socket){
        this.str = text;
        connectDevice = device;
        connectSocket = socket;
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_home, container, false);

        abutton = view.findViewById(R.id.button2);

        abutton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
//                deviceFragment df = new deviceFragment();
//                deviceFragment.ConnectThread fg = new deviceFragment.ConnectThread();
//                deviceFragment.connectThread.send();
//                df.data = "heloow";
//                df.send();
//                Toast.makeText(getActivity(), str, Toast.LENGTH_SHORT).show();

                if (connectSocket == null) {
                    Toast.makeText(getActivity(), "Please Connect to device", Toast.LENGTH_SHORT).show();
                }
                else {
                    try {
                        OutputStream outputStream = connectSocket.getOutputStream();
                        outputStream.write(32);
                        outputStream.write(49);
                    } catch (IOException e) {
                        Toast.makeText(getActivity(), "output " + e, Toast.LENGTH_SHORT).show();
                    }
                }
            }
        });




        return view;
    }
}