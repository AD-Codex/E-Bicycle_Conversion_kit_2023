package com.example.e_bicycle_v1;

import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.app.Activity;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.Point;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.dnn.Dnn;
import org.opencv.dnn.Net;
import org.opencv.imgproc.Imgproc;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.ArrayList;

public class OpenCvCam extends Activity implements CameraBridgeViewBase.CvCameraViewListener2{

    TextView textView;
    Button button;
    ArrayList<String> classList = new ArrayList<String>();
    private Net net;
    CameraBridgeViewBase cameraBridgeViewBase;
    BluetoothDevice connectDevice;
    BluetoothSocket connectSocket;
    OutputStream outputStream;
    InputStream inputStream;
    int Stop = 0;
    int status = 0;

    BaseLoaderCallback baseLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status){
                case LoaderCallbackInterface.SUCCESS:{
                    Log.i("TAG","onManagerConnected: opencv loaded");
                    cameraBridgeViewBase.enableView();
                }
                default:{
                    super.onManagerConnected(status);
                }
                break;
            }
        }
    };


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ActivityCompat.requestPermissions(OpenCvCam.this, new String[]{Manifest.permission.CAMERA},1);
        setContentView(R.layout.activity_open_cv_cam);

        button = findViewById(R.id.button6);
        textView = findViewById(R.id.textView);


        cameraBridgeViewBase = (CameraBridgeViewBase) findViewById(R.id.camera_surface);
        cameraBridgeViewBase.setVisibility(SurfaceView.VISIBLE);
        cameraBridgeViewBase.setCvCameraViewListener(this);

        Toast.makeText(this, "Please place Phone vertical landscape mode", Toast.LENGTH_SHORT).show();

        showAfterDelay(this, 5000, "Auto Mode Starting Now");

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                connectDevice = HomeFragment.connectDevice;
                connectSocket = HomeFragment.connectSocket;
                outputStream = HomeFragment.outputStream;
                inputStream = HomeFragment.inputStream;

                HomeFragment homeFragment = new HomeFragment(connectDevice, connectSocket, inputStream, outputStream);
                homeFragment.A_Stop();
            }
        });

        // blu send data loop ------------------------
        start();


    }



    // Start up setting ----------------------------------------------------------------------------
    public void onCameraViewStarted(int width, int height) {
        String model1 = getPath("frozen_inference_graph.pb", this);
        String config = getPath("ssd_mobilenet_v3_large_coco_2020_01_14.pbtxt", this);

        // object detect
        net = Dnn.readNet( model1, config);
        Log.i("TAG", "Network loaded successfully " + net);
        Toast.makeText(this, "Network loaded successfully", Toast.LENGTH_SHORT).show();

        String string = "";
        InputStream is = this.getResources().openRawResource(R.raw.labels);
        BufferedReader reader = new BufferedReader(new InputStreamReader(is));
        while (true) {
            try {
                if ((string = reader.readLine()) == null)
                    break;
            }
            catch (IOException e) {
                e.printStackTrace();
            }
            classList.add(string);
            Log.d("TAG", string);
        }
        Toast.makeText(this, "Class list uploaded", Toast.LENGTH_SHORT).show();
    }
    // ---------------------------------------------------------------------------------------------



    // Cam frame Loop ------------------------------------------------------------------------------
    public Mat onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame inputFrame) {
        final int IN_WIDTH = 300;
        final int IN_HEIGHT = 300;
        final float WH_RATIO = (float)IN_WIDTH / IN_HEIGHT;
        final double IN_SCALE_FACTOR = 1/127.5;
        final double MEAN_VAL = 127.5;
        final double THRESHOLD = 0.5;
        final int leftLim = 312;
        final int rightLim = 708;

        Scalar Red = new Scalar(255,0,0);
        Scalar Green = new Scalar(0,255,0);
        Scalar Yellow = new Scalar(255,255,0);
        Scalar color;

        // Get a new frame
        Mat frame = inputFrame.rgba();
        Imgproc.cvtColor(frame, frame, Imgproc.COLOR_RGBA2RGB);

        // Forward image through network.
        Mat blob = Dnn.blobFromImage( frame, IN_SCALE_FACTOR, new Size(IN_WIDTH, IN_HEIGHT), new Scalar(127.5, 127.5, 127.5), /*swapRB*/true, /*crop*/false);

        net.setInput(blob);
        Mat detections = net.forward();
        int cols = frame.cols();
        int rows = frame.rows();
        detections = detections.reshape(1, (int)detections.total() / 7);

        String objects = "";
        Stop = 0;

        for (int i = 0; i < detections.rows(); ++i) {
            double confidence = detections.get(i, 2)[0];
            if (confidence > THRESHOLD) {
                int classId = (int)detections.get(i, 1)[0];
                int left = (int)(detections.get(i, 3)[0] * cols)-25;
                int top = (int)(detections.get(i, 4)[0] * rows)-20;
                int right = (int)(detections.get(i, 5)[0] * cols)-10;
                int bottom = (int)(detections.get(i, 6)[0] * rows);

                String label = classList.get(classId-1) + " " + String.format("%.2f", confidence);

                // limit define --------------------------------------------------------------------
                if ( left > leftLim && left < rightLim ) {
                    color = Yellow;
//                    objects = objects + label + "\n";
                } else if ( right > leftLim && right < rightLim)  {
                    color = Yellow;
//                    objects = objects + label + "\n";
                } else if ( left < leftLim && right > rightLim) {
                    color = Yellow;
//                    objects = objects + label + "\n";
                } else {
                    color = Green;
                }

                // detect on path
                // A=(509.804,327.176) B=(227,760) C=(793,760)
                if ( isInside(510,327,135,900,884,900,(float) left,(float) bottom)) {
                    color = Red;
                } else if (isInside(510,327,135,900,884,900,(float) right,(float) bottom)) {
                    color = Red;
                } else if (bottom > 500 )  {
                    if (left < (bottom-1107.17)/(-1.53) && right > (bottom+452.82)/1.53) {
                        color = Red;
                    }
                }

                if ( color == Red) {
                    int[] vehicles = {1,2,3,4,5,6,7,8};
                    for (int vehicle : vehicles) {
                        if ( classId == vehicle ) {
//                            objects = objects +  classList.get(classId-1) + " " + (bottom-top) + "\n";

                            if (bottom > 500) {
                                Stop = 1;
                                objects = objects +  classList.get(classId-1) + " " + (bottom-top) + "\n";
                            }
                        }
                    }
                }
                // ---------------------------------------------------------------------------------


                // Draw rectangle around detected object.
                Imgproc.rectangle(frame, new Point(left, top), new Point(right, bottom), color, 3);

                Log.d("TAG", label);
                Imgproc.putText(frame, label, new Point(left+10, top+10), Core.FONT_HERSHEY_SIMPLEX, 1, new Scalar(0, 255, 255), 3);
            }
        }

//        try {
//            if (Stop == 1) {
//                objects = objects + "stop =1" + "\n";
//                connectDevice = HomeFragment.connectDevice;
//                connectSocket = HomeFragment.connectSocket;
//                outputStream = HomeFragment.outputStream;
//                inputStream = HomeFragment.inputStream;
//
//                HomeFragment homeFragment = new HomeFragment(connectDevice, connectSocket, inputStream, outputStream);
//                homeFragment.S_Stop();
//            }
//            else {
//                objects = objects + "stop =0" + "\n";
//                connectDevice = HomeFragment.connectDevice;
//                connectSocket = HomeFragment.connectSocket;
//                outputStream = HomeFragment.outputStream;
//                inputStream = HomeFragment.inputStream;
//
//                HomeFragment homeFragment = new HomeFragment(connectDevice, connectSocket, inputStream, outputStream);
//                homeFragment.R_Run();
//            }
//        } catch (Exception e) {
//            Log.d("TAG", ""+e);;
//        }


        // limit lines
        Imgproc.line(frame, new Point(397,500), new Point(227,760), new Scalar(255, 255, 255), 2);
        Imgproc.line(frame, new Point(623,500), new Point(793,760), new Scalar(255, 255, 255), 2);

        textView.setText(objects);

        return frame;
    }
    // ---------------------------------------------------------------------------------------------



    // Upload file----------------------------------------------------------------------------------
    private static String getPath(String file, Context context) {
        AssetManager assetManager = context.getAssets();
        BufferedInputStream inputStream = null;
        try {
            // Read data from assets.
            inputStream = new BufferedInputStream(assetManager.open(file));
            byte[] data = new byte[inputStream.available()];
            inputStream.read(data);
            inputStream.close();
            // Create copy file in storage.
            File outFile = new File(context.getFilesDir(), file);
            FileOutputStream os = new FileOutputStream(outFile);
            os.write(data);
            os.close();
            // Return a path to file which may be read in common way.
            return outFile.getAbsolutePath();
        } catch (IOException ex) {
            Log.i("TAG", "Failed to upload a file");
            Toast.makeText(context, "Failed to upload a file", Toast.LENGTH_SHORT).show();
        }
        return "";
    }
    // ---------------------------------------------------------------------------------------------


    // Area cal-------------------------------------------------------------------------------------
    public static float areaTr(float x1, float y1 , float x2, float y2, float x3, float y3){
        return Math.abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2);
    }
    public static boolean isInside(float x1, float y1 , float x2, float y2, float x3, float y3, float pointX, float pointY) {
        float A = areaTr(x1, y1, x2, y2, x3, y3);
        float A1 = areaTr(pointX, pointY, x2, y2, x3, y3);
        float A2 = areaTr(x1, y1, pointX, pointY, x3, y3);
        float A3 = areaTr(x1, y1, x2, y2, pointX, pointY);
        return (A == A1 + A2 + A3);
    }
    // ---------------------------------------------------------------------------------------------


    public void showAfterDelay(Context context, int milli, String text){
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {

                Toast.makeText(context,text,Toast.LENGTH_SHORT).show();
                BluetoothDevice connectDevice = HomeFragment.connectDevice;
                BluetoothSocket connectSocket = HomeFragment.connectSocket;
                OutputStream outputStream = HomeFragment.outputStream;
                InputStream inputStream = HomeFragment.inputStream;

                HomeFragment homeFragment = new HomeFragment(connectDevice, connectSocket, inputStream, outputStream);
                homeFragment.A_Start();
            }
        },milli);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
//        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        // camera request
        switch (requestCode){
            case 1:{
                if(grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED){
//                    cameraBridgeViewBase.setCameraPermissionGranted();
                }
                else{

                }
                return;
            }
        }
    }


    @Override
    protected void onResume() {
        super.onResume();
        if(OpenCVLoader.initDebug()){
            Log.d("TAG","onResume: opencv initialized");
            baseLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS);
        }
        else {
            Log.d("TAG","onResume: opencv not initialized");
            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION, this, baseLoaderCallback);
        }
    }


    @Override
    protected void onPause() {
        super.onPause();
        if(cameraBridgeViewBase != null){
            cameraBridgeViewBase.disableView();
        }
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(cameraBridgeViewBase != null){
            cameraBridgeViewBase.disableView();
        }
    }


    @Override
    public void onCameraViewStopped() {

    }

    private Handler handler = new Handler();
    private Runnable runnable = new Runnable() {
        @Override
        public void run() {
            try {
                if (Stop == 1) {
                    connectDevice = HomeFragment.connectDevice;
                    connectSocket = HomeFragment.connectSocket;
                    outputStream = HomeFragment.outputStream;
                    inputStream = HomeFragment.inputStream;

                    HomeFragment homeFragment = new HomeFragment(connectDevice, connectSocket, inputStream, outputStream);
                    homeFragment.S_Stop();
                }
                else {
                    connectDevice = HomeFragment.connectDevice;
                    connectSocket = HomeFragment.connectSocket;
                    outputStream = HomeFragment.outputStream;
                    inputStream = HomeFragment.inputStream;

                    HomeFragment homeFragment = new HomeFragment(connectDevice, connectSocket, inputStream, outputStream);
                    homeFragment.R_Run();
                }
            } catch (Exception e) {
                Log.d("TAG", ""+e);;
            }
            start();
        }
    };

    public void start(){
        handler.postDelayed(runnable, 500);
    }
    // ---------------------------------------------------------------------------------------------


}