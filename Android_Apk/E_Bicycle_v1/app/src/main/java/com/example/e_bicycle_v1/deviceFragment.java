package com.example.e_bicycle_v1;

import static android.app.Activity.RESULT_OK;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.ListView;
import android.widget.Switch;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

public class deviceFragment extends Fragment {

    private static final int REQUEST_ENABLE_BT = 0;
    private BluetoothAdapter bluetoothAdapter;
    private Context context;

    Switch aSwitch;
    Button findDevice;
    ListView listView;

    String[]  countryList = new String[]{"India", "China", "America"};



    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {


        // Bluetooth Adepter config
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();


        // Widget setup
        View view = inflater.inflate(R.layout.fragment_device, container, false);
        aSwitch = view.findViewById(R.id.switch4);
        findDevice = view.findViewById(R.id.button);
        listView = view.findViewById(R.id.listview_item);

//        ArrayAdapter<String > adapter = new ArrayAdapter<String>(getActivity().getApplicationContext(), android.R.layout.simple_list_item_1,countryList);
//        listView.setAdapter(adapter);



        // Check the BLU state in mobile
        if (!bluetoothAdapter.isEnabled()) {
            aSwitch.setChecked(false);
        }
        else {
            aSwitch.setChecked(true);
        }

        // BLU switch function
        aSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @SuppressLint("MissingPermission")
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if  (aSwitch.isChecked()){
                    // BLU Enable
                    Intent enableBtIntent = new Intent(bluetoothAdapter.ACTION_REQUEST_ENABLE);
                    startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
                }
                else {
                    // BLU disable
                    bluetoothAdapter.disable();
                }
            }
        });


        // Connect to device
        findDevice.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                if (bluetoothAdapter.isEnabled()) {
                    // get the paired devices
                    Set<BluetoothDevice> pairDevices = bluetoothAdapter.getBondedDevices();
                    if (pairDevices.size() > 0) {
                        List<String> s = new ArrayList<String>();
                        for (BluetoothDevice device: pairDevices) {
                            s.add(device.getName());
                        }
                        ArrayAdapter<String > adapter = new ArrayAdapter<String>(getActivity().getApplicationContext(), android.R.layout.simple_list_item_1, s);
                        listView.setAdapter(adapter);

                    }
                    else {
                        // No pair devices
                        String[]  noPairDevice = new String[]{"No Pair Devices"};
                        ArrayAdapter<String > adapter = new ArrayAdapter<String>(getActivity().getApplicationContext(), android.R.layout.simple_list_item_1,noPairDevice);
                        listView.setAdapter(adapter);
                    }
                }
                else {
                    // Bluetooth not enable
                    String[]  noBlueOn = new String[]{"Please turn on Bluetooth"};
                    ArrayAdapter<String > adapter = new ArrayAdapter<String>(getActivity().getApplicationContext(), android.R.layout.simple_list_item_1,noBlueOn);
                    listView.setAdapter(adapter);
                }

            }

        });


        return view;

    }


    // Request Code function
    @Override
    public void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        switch (requestCode) {
            case REQUEST_ENABLE_BT:
                if (resultCode == RESULT_OK){
                }
                else {
                    aSwitch.setChecked(false);
                }
        }
        super.onActivityResult(requestCode, resultCode, data);
    }
}