package com.example.e_bicycle_v1;

import static android.app.Activity.RESULT_OK;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;

import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.localbroadcastmanager.content.LocalBroadcastManager;

import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.ListView;
import android.widget.Switch;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.UUID;

public class deviceFragment extends Fragment {

    private static final int REQUEST_ENABLE_BT = 0;
    private BluetoothAdapter bluetoothAdapter;
    public ArrayList<BluetoothDevice> bluArray = new ArrayList<>();
    public static BluetoothDevice connectDevice;
    public static BluetoothSocket connectSocket;
    private Context context;
    public static ConnectThread connectThread;
    private BluetoothSocket mmSocket;
    public String data = "hola";


    Switch aSwitch;
    Button findDevice;
    ListView listView;
    List<String> listDevices = new ArrayList<String>();
    List<String> pair_Device = new ArrayList<String>();
    List<String> pair_Mac = new ArrayList<String>();
    List<String> scanDevices = new ArrayList<String>();
    List<String> scanMac = new ArrayList<String>();
    int pairCount = 0;
    ArrayAdapter<String > adapter;
    String str123="";


    public deviceFragment(BluetoothDevice device, BluetoothSocket socket) {
        // Required empty public constructor
        connectDevice = device;
        connectSocket = socket;
    }



    // On Create function --------------------------------------------------------------------------
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {

        // Bluetooth Adepter config
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        // Widget setup
        View view = inflater.inflate(R.layout.fragment_device, container, false);
        aSwitch = view.findViewById(R.id.switch4);
        findDevice = view.findViewById(R.id.button);
        listView = view.findViewById(R.id.listview_item);

        // Check the BLU state in mobile
        if (!bluetoothAdapter.isEnabled()) {
            aSwitch.setChecked(false);
        }
        else {
            aSwitch.setChecked(true);
        }


        // BLU switch click function ---------------------------------------------------------------
        aSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @SuppressLint("MissingPermission")
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if  (aSwitch.isChecked()){
                    // BLU Enable
                    Intent enableBtIntent = new Intent(bluetoothAdapter.ACTION_REQUEST_ENABLE);
                    startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);

                    Toast.makeText(getActivity(), "Bluetooth Enable", Toast.LENGTH_SHORT).show();
                }
                else {
                    // BLU disable
                    bluetoothAdapter.disable();

                    Toast.makeText(getActivity(), "Bluetooth Disable", Toast.LENGTH_SHORT).show();
                }
            }
        });
        // -----------------------------------------------------------------------------------------


        // Connect to device function --------------------------------------------------------------
        findDevice.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                data = "hello";

                bluArray.clear();
                listDevices.clear();
                scanDevices.clear();
                scanMac.clear();
                pair_Device.clear();
                pair_Mac.clear();
                pairCount = 0;
                adapter = new ArrayAdapter<String>(getActivity().getApplicationContext(), android.R.layout.simple_list_item_1, listDevices);
                listView.setAdapter(adapter);

                // Pair devices
                if (bluetoothAdapter.isEnabled()) {
                    // get the paired devices
                    listDevices.add("Paired Devices -----------------------------------------");
                    pairCount = pairCount + 1;
                    listView.setAdapter(adapter);

                    Set<BluetoothDevice> pairDevices = bluetoothAdapter.getBondedDevices();
                    if (pairDevices.size() > 0) {
                        for (BluetoothDevice device: pairDevices) {
                            bluArray.add(device);
                            listDevices.add("   > " + device.getName());
                            pair_Device.add(device.getName());
                            pair_Mac.add(device.getAddress());
                            pairCount = pairCount + 1;
                        }
                        listView.setAdapter(adapter);
                    }
                    else {
                        // No pair devices
                        listDevices.add("-- No Paired Devices");
                        pairCount = pairCount + 1;
                        listView.setAdapter(adapter);
                    }

                    // Scan devices
                    listDevices.add("Discover Devices -------------------------------------");
                    pairCount = pairCount + 1;
                    listView.setAdapter(adapter);

                    bluetoothAdapter.startDiscovery();
                }
                else {
                    // Bluetooth not enable
                    Toast.makeText(getActivity(), "Please turn on Bluetooth", Toast.LENGTH_SHORT).show();
                }

            }

        });


        // list view click function ----------------------------------------------------------------
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {

                if (bluetoothAdapter.isEnabled()) {
                    if (i>0 ) {
                        if (i+1<pairCount && pair_Device.size()>0){
                            String str = bluArray.get(i-1).getName().toString() + " : " + bluArray.get(i-1).getAddress().toString();
                            Toast.makeText(getActivity(), str, Toast.LENGTH_SHORT).show();
                        }
                        else if (i+1>pairCount && scanDevices.size()>0) {
                            String str = bluArray.get(i-2).getName().toString() + " : " + bluArray.get(i-2).getAddress().toString();
                            Toast.makeText(getActivity(), str, Toast.LENGTH_SHORT).show();

//                            ConnectThread connectThread = new ConnectThread(bluArray.get(i-2));
//                            connectThread.start();
//                            connectThread.send();
//
//                            MainActivity ma = new MainActivity();
//                            ma.update(connectDevice, connectSocket);


                            UUID uuid = UUID.fromString("beb5483e-36e1-4688-b7f5-ea07361b26a8");
                            try {
                                uuid = bluArray.get(i-2).getUuids()[0].getUuid();
//                                UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
                                Toast.makeText(getActivity(), "new "+uuid, Toast.LENGTH_SHORT).show();
                            } catch (Exception e) {
                                Toast.makeText(getActivity(), "uuid error", Toast.LENGTH_SHORT).show();
                            }

                            BluetoothSocket btSocket = null;
                            int count = 0;
                            do {
                                try {
                                    btSocket = bluArray.get(i - 2).createRfcommSocketToServiceRecord(uuid);
                                    Toast.makeText(getActivity(), ""+btSocket, Toast.LENGTH_SHORT).show();
                                    btSocket.connect();
                                    Toast.makeText(getActivity(), ""+btSocket.isConnected(), Toast.LENGTH_SHORT).show();
                                } catch (IOException e) {
                                    Toast.makeText(getActivity(), "error connect"+e, Toast.LENGTH_SHORT).show();
                                }
                                count++;
                            } while (!btSocket.isConnected() && count <3);

                            try {
                                OutputStream outputStream = btSocket.getOutputStream();
                                outputStream.write(49);
                                for (int j=0;j<10;j=j+1) {
                                    outputStream.write(32);
                                    outputStream.write(50+j);
                                }
                            } catch (IOException e) {
                                Toast.makeText(getActivity(), "output "+e, Toast.LENGTH_SHORT).show();
                            }

//                            InputStream inputStream = null;
//                            try{
//                                inputStream = btSocket.getInputStream();
//                                inputStream.skip(inputStream.available());
//
//                                String str1 = "";
//                                for (int j=0; j<26;j++) {
//                                    byte b = (byte) inputStream.read();
//                                    str1 = str1 + (char) b;
//                                }
//                                Toast.makeText(getActivity(), "in "+str1, Toast.LENGTH_SHORT).show();
//                            } catch (IOException e) {
//                                Toast.makeText(getActivity(), "input "+e, Toast.LENGTH_SHORT).show();
//                            }



                            try {
                                btSocket.close();
                            } catch (IOException e) {
                                Toast.makeText(getActivity(), "error disconnect", Toast.LENGTH_SHORT).show();
                            }


                        }

                    }
                }


            }
        });

        return view;
    }




    // Broadcast Receiver --------------------------------------------------------------------------
    private final BroadcastReceiver broadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();

            if (BluetoothDevice.ACTION_FOUND.equals(action)){
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                bluArray.add(device);
                scanMac.add(device.getAddress());
                scanDevices.add(device.getName());
                listDevices.add("   > " + device.getName());
            }
            if (scanDevices.size() == 0){
                listDevices.add("-- No Device Found");
            }
            listView.setAdapter(adapter);
        }
    };

    @Override
    public void onDestroy() {
        super.onDestroy();

        // Unregister for broadcasts
        requireActivity().unregisterReceiver(broadcastReceiver);
    }

    @Override
    public void onStart() {
        super.onStart();

        // Register for broadcasts when a device is discovered.
        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        requireActivity().registerReceiver(broadcastReceiver, filter);
    }


    // Request Code function -----------------------------------------------------------------------
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



    // BLE connect to device -----------------------------------------------------------------------
    public class ConnectThread extends Thread {
        private BluetoothSocket mmSocket;
        public ConnectThread(BluetoothDevice device) {
//            BluetoothSocket tmp = null;
//            BluetoothDevice mmDevice = null;
//            mmDevice = device;
            connectDevice = device;
            UUID uuid = connectDevice.getUuids()[0].getUuid();
//            UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
            Toast.makeText(getActivity(), " "+uuid, Toast.LENGTH_SHORT).show();

            try {
                connectSocket = connectDevice.createRfcommSocketToServiceRecord(uuid);
                Toast.makeText(getActivity(), ""+connectSocket, Toast.LENGTH_SHORT).show();
                connectSocket.connect();
                Toast.makeText(getActivity(), "connect to device "+connectSocket.isConnected(), Toast.LENGTH_SHORT).show();
            } catch (IOException e) {
                Toast.makeText(getActivity(), "Socket's create() method failed", Toast.LENGTH_SHORT).show();
            }

            mmSocket = connectSocket;

        }

        public void send() {
            try {
                OutputStream outputStream = mmSocket.getOutputStream();
                outputStream.write(32);
                outputStream.write(49);
            } catch (IOException e) {
                Toast.makeText(getActivity(), "output "+e, Toast.LENGTH_SHORT).show();
            }
        }

        public void cancel() {
            try {
                mmSocket.close();
            }
            catch (IOException e) {
                Toast.makeText(getActivity(), "Could not close the client socket", Toast.LENGTH_SHORT).show();
            }
        }


    }


}