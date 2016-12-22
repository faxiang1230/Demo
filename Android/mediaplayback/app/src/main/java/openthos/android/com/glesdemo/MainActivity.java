package openthos.android.com.glesdemo;



import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;

import java.io.IOException;

import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends AppCompatActivity {
    private Intent intent = new Intent("openthos.android.com.glesdemo.MUSIC");
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                        WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_main);
        Button playbutton = (Button) findViewById(R.id.playbtn);
        playbutton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Perform action on click
                intent.setPackage(getPackageName());
                startService(intent);
            }
        });
    }
}
