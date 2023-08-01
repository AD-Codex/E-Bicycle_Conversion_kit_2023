package com.example.e_bicycle_v1;

import androidx.annotation.NonNull;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.view.GravityCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.MenuItem;
import android.widget.ArrayAdapter;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.material.navigation.NavigationView;

import org.opencv.android.OpenCVLoader;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

//    // checking opencv installed
//    static {
//        if (OpenCVLoader.initDebug()){
//            Log.d("TAG","Opencv installed");
//        }
//        else {
//            Log.d("TAG","Opencv not installed");
//        }
//    }

    private BluetoothAdapter bluetoothAdapter;
    public static BluetoothDevice connectDevice = null;
    public static BluetoothSocket connectSocket = null;
    public static OutputStream outputStream = null;
    public static InputStream inputStream = null;
    public UUID deviceUuid;
    private Context context;
    public static String  text = "main";
    public static int elec_mode = 0;
    public static int auto_mode=0;


    DrawerLayout drawerLayout;
    Toolbar toolbar;
    NavigationView navigationView;
    TextView textView;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // toolbar setup
        toolbar = findViewById(R.id.toolbar);
        drawerLayout = findViewById(R.id.drawerLayout);
        navigationView = findViewById(R.id.nav);


        // toggle button
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle( this,drawerLayout,toolbar,R.string.open,R.string.close);
        drawerLayout.addDrawerListener(toggle);
        toggle.syncState();
        toggle.getDrawerArrowDrawable().setColor(getResources().getColor(R.color.white));

        // Home page call
        fragmentR(new HomeFragment(connectDevice, connectSocket, inputStream, outputStream));


        // Bluetooth adepter check
        initBluetooth();



        // Page change -----------------------------------------------------------------------------
        navigationView.setNavigationItemSelectedListener(new NavigationView.OnNavigationItemSelectedListener() {
            @Override
            public boolean onNavigationItemSelected(@NonNull MenuItem item) {

                switch (item.getItemId()){
                    case R.id.Home:
                        // Home page Bluetooth
                        fragmentR(new HomeFragment(connectDevice, connectSocket, inputStream, outputStream));

                        drawerLayout.closeDrawer(GravityCompat.START);
                        Toast.makeText(MainActivity.this, "Home", Toast.LENGTH_SHORT).show();
                        break;

                    case R.id.Location:
                        // Location
                        fragmentR(new locationFragment());

                        drawerLayout.closeDrawer(GravityCompat.START);
                        Toast.makeText(MainActivity.this, "My Location", Toast.LENGTH_SHORT).show();
                        break;

                    case R.id.Device:
//                         Bluetooth
                        fragmentR(new deviceFragment(connectDevice, connectSocket));

                        drawerLayout.closeDrawer(GravityCompat.START);
                        Toast.makeText(MainActivity.this, "Scan Devices", Toast.LENGTH_SHORT).show();
                        break;

                    case R.id.Info:
                        fragmentR(new information());
                        drawerLayout.closeDrawer(GravityCompat.START);
                        Toast.makeText(MainActivity.this, "Info", Toast.LENGTH_SHORT).show();
                        break;

                }

                return true;
            }
        });
        // -----------------------------------------------------------------------------------------


    }




    // fragment call--------------------------------------------------------------------------------
    private void fragmentR(Fragment fragment){
        FragmentManager fragmentManager = getSupportFragmentManager();
        FragmentTransaction fragmentTransaction = fragmentManager.beginTransaction();
        fragmentTransaction.replace(R.id.frameLayout, fragment);
        fragmentTransaction.commit();
    }
    // ---------------------------------------------------------------------------------------------



    // Bluetooth init-------------------------------------------------------------------------------
    private void initBluetooth() {
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if ( bluetoothAdapter == null) {
            showAfterDelay(getApplicationContext(), 2000, "No Bluetooth Adepter available");
        }
        else{
            showAfterDelay(getApplicationContext(), 2000, "Bluetooth Adepter available");
        }
    }
    // ---------------------------------------------------------------------------------------------



    // message show --------------------------------------------------------------------------------
    public void showAfterDelay(Context context, int milli, String text){
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(context,text,Toast.LENGTH_SHORT).show();
            }
        },milli);
    }
    // ---------------------------------------------------------------------------------------------



    //  Connect to Bluetooth Device ----------------------------------------------------------------
    public void connectDevice(ArrayList<BluetoothDevice> bluArray, int i) {
        String status = " ";
        UUID deviceUuid = null;
        BluetoothSocket btSocket = null;

        try {
            deviceUuid = bluArray.get(i-2).getUuids()[0].getUuid();
            Log.d("TAG", "new "+ deviceUuid);

            int count = 0;
            do {
                try {
                    btSocket = bluArray.get(i - 2).createRfcommSocketToServiceRecord(deviceUuid);
                    btSocket.connect();
                    status = "btSocket connect "+btSocket.isConnected();
                    Log.d("TAG", status);

                } catch (IOException e) {
                    status = "error connect "+e;
                    Log.d("TAG", status);
                }
                count++;
            } while (!btSocket.isConnected() && count <3);

        } catch (Exception e) {
            Log.d("TAG", "error "+e);
        }
        connectSocket = btSocket;

        try {
            OutputStream outStream = btSocket.getOutputStream();
            InputStream inStream = btSocket.getInputStream();

            String msg = "phone connected";
            byte[] bytes = msg.getBytes(StandardCharsets.US_ASCII);
            outStream.write(10);
            for (byte aByte : bytes) {
                outStream.write(aByte);
            }
            outStream.write(10);

            outStream.write(66);

            inputStream = inStream;
            outputStream = outStream;

        } catch (IOException e) {
            Log.d("TAG", "error sending data");
        }

    }
    // ---------------------------------------------------------------------------------------------

    public void update(BluetoothDevice device, BluetoothSocket socket) {
        text = "edit";
        connectDevice = device;
        connectSocket = socket;
        Log.d("TAG", text);
    }

}