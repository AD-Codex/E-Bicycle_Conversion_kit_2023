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

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    private BluetoothAdapter bluetoothAdapter;
    public static BluetoothDevice connectDevice = null;
    public static BluetoothSocket connectSocket = null;
    private Context context;
    public static String  text = "main";


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
        fragmentR(new HomeFragment(text, connectDevice, connectSocket));


        // Bluetooth adepter check
        initBluetooth();



        // Page change
        navigationView.setNavigationItemSelectedListener(new NavigationView.OnNavigationItemSelectedListener() {
            @Override
            public boolean onNavigationItemSelected(@NonNull MenuItem item) {

                switch (item.getItemId()){
                    case R.id.Home:
                        // Home page Bluetooth
                        fragmentR(new HomeFragment(text, connectDevice, connectSocket));

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
                }

                return true;
            }
        });


    }




    // fragment call
    private void fragmentR(Fragment fragment){
        FragmentManager fragmentManager = getSupportFragmentManager();
        FragmentTransaction fragmentTransaction = fragmentManager.beginTransaction();
        fragmentTransaction.replace(R.id.frameLayout, fragment);
        fragmentTransaction.commit();
    }



    // Bluetooth init
    private void initBluetooth() {
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if ( bluetoothAdapter == null) {
            showAfterDelay(getApplicationContext(), 2000, "No Bluetooth Adepter available");
        }
        else{
            showAfterDelay(getApplicationContext(), 2000, "Bluetooth Adepter available");
        }
    }



    // message show
    public void showAfterDelay(Context context, int milli, String text){
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(context,text,Toast.LENGTH_SHORT).show();
            }
        },milli);
    }

    public void update(BluetoothDevice device, BluetoothSocket socket) {
        text = "edit";
        connectDevice = device;
        connectSocket = socket;
        Log.d("TAG", text);
    }

}