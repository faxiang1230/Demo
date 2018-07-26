package com.example.fox;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Locale;

import android.app.Activity;
import android.app.ActionBar;
import android.app.Fragment;
import android.app.FragmentManager;
import android.app.FragmentTransaction;
import android.content.Context;
import android.support.annotation.Nullable;
import android.support.v13.app.FragmentPagerAdapter;
import android.os.Bundle;
import android.support.v4.view.ViewPager;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.ScrollView;
import android.widget.TextView;

public class MainActivity extends Activity implements ActionBar.TabListener {

	/**
	 * The {@link android.support.v4.view.PagerAdapter} that will provide
	 * fragments for each of the sections. We use a {@link FragmentPagerAdapter}
	 * derivative, which will keep every loaded fragment in memory. If this
	 * becomes too memory intensive, it may be best to switch to a
	 * {@link android.support.v13.app.FragmentStatePagerAdapter}.
	 */
	private static String TAG = "TEST";
	private static String CONFPATH = "/data/local/tmp/sys.config";
	private static String LOGPATH = "conf";
	public Fragment mConfigure;
	public Fragment mLog;
	private LinearLayout mConfigureLayout;
	private LinearLayout mLogLayout;
	public TextView tv;
	public Button saveBtn;
	public EditText ispText;
	public EditText webipText;
	public EditText miscText;
	SectionsPagerAdapter mSectionsPagerAdapter;

	/**
	 * The {@link ViewPager} that will host the section contents.
	 */
	ViewPager mViewPager;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		mConfigure = new ConfigureFragment();
		mLog = new LogFragment();
		// Set up the action bar.
		final ActionBar actionBar = getActionBar();
		actionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_TABS);

		// Create the adapter that will return a fragment for each of the three
		// primary sections of the activity.
		mSectionsPagerAdapter = new SectionsPagerAdapter(getFragmentManager());
		// Set up the ViewPager with the sections adapter.
		mViewPager = (ViewPager) findViewById(R.id.pager);
		mViewPager.setAdapter(mSectionsPagerAdapter);

		// When swiping between different sections, select the corresponding
		// tab. We can also use ActionBar.Tab#select() to do this if we have
		// a reference to the Tab.
		mViewPager
				.setOnPageChangeListener(new ViewPager.SimpleOnPageChangeListener() {
					@Override
					public void onPageSelected(int position) {
						actionBar.setSelectedNavigationItem(position);
					}
				});

		actionBar.addTab(actionBar.newTab().setText("Configure")
				.setTabListener(this));
		actionBar
				.addTab(actionBar.newTab().setText("Log").setTabListener(this));
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	@Override
	public void onTabSelected(ActionBar.Tab tab,
			FragmentTransaction fragmentTransaction) {
		// When the given tab is selected, switch to the corresponding page in
		// the ViewPager.
		// if(tab.getPosition() == 1) {
		// TextView tvv = (TextView) findViewById(R.id.logView);
		// tvv.setText("nicai");
		// }
		mViewPager.setCurrentItem(tab.getPosition());
	}

	@Override
	public void onTabUnselected(ActionBar.Tab tab,
			FragmentTransaction fragmentTransaction) {
	}

	@Override
	public void onTabReselected(ActionBar.Tab tab,
			FragmentTransaction fragmentTransaction) {
	}

	/**
	 * A {@link FragmentPagerAdapter} that returns a fragment corresponding to
	 * one of the sections/tabs/pages.
	 */
	public class SectionsPagerAdapter extends FragmentPagerAdapter {

		public SectionsPagerAdapter(FragmentManager fm) {
			super(fm);
		}

		@Override
		public Fragment getItem(int position) {
			// getItem is called to instantiate the fragment for the given page.
			// Return a PlaceholderFragment (defined as a static inner class
			// below).
			if (position == 0) {
				return mConfigure;
			} else if (position == 1) {
				// updateLog();
				return mLog;
			}
			return PlaceholderFragment.newInstance(position + 1);
		}

		@Override
		public int getCount() {
			// Show 3 total pages.
			return 2;
		}

		@Override
		public CharSequence getPageTitle(int position) {
			Locale l = Locale.getDefault();
			switch (position) {
			case 0:
				return getString(R.string.title_section1).toUpperCase(l);
			case 1:
				return getString(R.string.title_section2).toUpperCase(l);
			case 2:
				return getString(R.string.title_section3).toUpperCase(l);
			}
			return null;
		}
	}

	/**
	 * A placeholder fragment containing a simple view.
	 */
	public static class PlaceholderFragment extends Fragment {
		/**
		 * The fragment argument representing the section number for this
		 * fragment.
		 */
		private static final String ARG_SECTION_NUMBER = "section_number";

		/**
		 * Returns a new instance of this fragment for the given section number.
		 */
		public static PlaceholderFragment newInstance(int sectionNumber) {
			PlaceholderFragment fragment = new PlaceholderFragment();
			Bundle args = new Bundle();
			args.putInt(ARG_SECTION_NUMBER, sectionNumber);
			fragment.setArguments(args);
			return fragment;
		}

		public PlaceholderFragment() {
		}

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container,
				Bundle savedInstanceState) {
			View rootView = inflater.inflate(R.layout.fragment_main, container,
					false);
			return rootView;
		}
	}

	public class ConfigureFragment extends Fragment {
		public View onCreateView(LayoutInflater inflater,
				@Nullable ViewGroup container,
				@Nullable Bundle savedInstanceState) {
			View view = inflater.inflate(R.layout.configure, container, false);
			saveBtn = (Button)view.findViewById(R.id.savebtn);
			ispText = (EditText) view.findViewById(R.id.ipEdit);
			webipText = (EditText) view.findViewById(R.id.webiptext);
			miscText = (EditText) view.findViewById(R.id.misctext);
			try {
				FileReader reader = new FileReader(CONFPATH);
				BufferedReader br = new BufferedReader(reader);
				String s;
				String[] sarray;
				if(br != null) {
					while(null != (s = br.readLine())) {
						sarray = s.split("\"");
						for(int i = 0; i < sarray.length; i++) {
							if(sarray[i].equals("misc"))
								miscText.setText(sarray[3]);
							if(sarray[i].equals("isp"))
								ispText.setText(sarray[3]);
							if(sarray[i].equals("webip"))
								webipText.setText(sarray[3]);
						}
					}
				}
			} catch (FileNotFoundException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			saveBtn.setOnClickListener(new View.OnClickListener(){

				@Override
				public void onClick(View v) {
					// TODO Auto-generated method stub
					FileWriter writer;
					try {
						writer = new FileWriter(CONFPATH);
						if (writer != null) {
							writer.write("{	\"misc\":	\"");
							writer.write(miscText.getText().toString());
							writer.write("\",\n\"isp\":	\"");
							writer.write(ispText.getText().toString());
							writer.write("\",\n\"webip\":    \"");
							writer.write(webipText.getText().toString());
							writer.write("\",\n\"auditnelk\":	30,\n"
									+ "\"msgnelk\":	31,\n"
									+ "\"polltime\":	10,\n"
									+ "\"regport\":	11901,\n"
									+ "\"auditport\":	11903,\n"
									+ "\"auditmax\":	104857600,\n"
									+ "\"encrypt\":	0,\n"
									+ "\"record\":   0\n}");
							writer.close();
						}
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}

				}
			});
			return view;
		}
	}

	public class LogFragment extends Fragment {
		public View onCreateView(LayoutInflater inflater,
				@Nullable ViewGroup container,
				@Nullable Bundle savedInstanceState) {
			View view = inflater.inflate(R.layout.log, container, false);
			tv = (TextView) view.findViewById(R.id.logView);
			StringBuffer sb = new StringBuffer();
			try {
				FileReader fr = new FileReader(CONFPATH);
				BufferedReader br = new BufferedReader(fr);
				String s;
				while(null != (s = br.readLine()))
					sb.append(s).append("\n");
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			tv.setText(sb.toString());
			tv.setSelected(true);
			return view;
		}
	}
}
