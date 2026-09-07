package git.artdeell.gl4eswrapper;


import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        PackageManager packageManager = getPackageManager();
        ApplicationInfo targetAppInfo = getAppicationInfo(packageManager);
        startActivity(packageManager.getLaunchIntentForPackage(targetAppInfo.packageName));
        finish();
    }

    public ApplicationInfo getAppicationInfo(PackageManager packageManager) {
        try {
            return packageManager.getApplicationInfo("git.artdeell.mjlaunch.debug", PackageManager.GET_SHARED_LIBRARY_FILES);
        }catch (PackageManager.NameNotFoundException e) {
            return null;
        }
    }
}