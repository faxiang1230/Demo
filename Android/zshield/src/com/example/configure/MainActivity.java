package com.example.configure;

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
import android.support.annotation.Nullable;
import android.support.v13.app.FragmentPagerAdapter;
import android.os.Bundle;
import android.support.v4.view.ViewPager;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends Activity implements ActionBar.TabListener {

	/**
	 * The {@link android.support.v4.view.PagerAdapter} that will provide fragments
	 * for each of the sections. We use a {@link FragmentPagerAdapter} derivative,
	 * which will keep every loaded fragment in memory. If this becomes too memory
	 * intensive, it may be best to switch to a
	 * {@link android.support.v13.app.FragmentStatePagerAdapter}.
	 */
	SectionsPagerAdapter mSectionsPagerAdapter;
	private static String PATH="/data/local/tmp/";
	private static String SYSCONF = "sys.config";
	private static String INFOLOG = "log_agent.txt";
	private static String DEBUGLOG = "log_monitor.txt";
	/**
	 * The {@link ViewPager} that will host the section contents.
	 */
	ViewPager mViewPager;
	private ConfigureFragment mcf;
	private LogFragment mlf;
	private LogFragment2 mlf2;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

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
		mViewPager.setOnPageChangeListener(new ViewPager.SimpleOnPageChangeListener() {
			@Override
			public void onPageSelected(int position) {
				actionBar.setSelectedNavigationItem(position);
			}
		});

		// For each of the sections in the app, add a tab to the action bar.
		for (int i = 0; i < mSectionsPagerAdapter.getCount(); i++) {
			// Create a tab with text corresponding to the page title defined by
			// the adapter. Also specify this Activity object, which implements
			// the TabListener interface, as the callback (listener) for when
			// this tab is selected.
			actionBar.addTab(actionBar.newTab().setText(mSectionsPagerAdapter.getPageTitle(i)).setTabListener(this));
		}
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
	public void onTabSelected(ActionBar.Tab tab, FragmentTransaction fragmentTransaction) {
		// When the given tab is selected, switch to the corresponding page in
		// the ViewPager.
		mViewPager.setCurrentItem(tab.getPosition());
	}

	@Override
	public void onTabUnselected(ActionBar.Tab tab, FragmentTransaction fragmentTransaction) {
	}

	@Override
	public void onTabReselected(ActionBar.Tab tab, FragmentTransaction fragmentTransaction) {
	}

	/**
	 * A {@link FragmentPagerAdapter} that returns a fragment corresponding to one
	 * of the sections/tabs/pages.
	 */
	public class SectionsPagerAdapter extends FragmentPagerAdapter {

		public SectionsPagerAdapter(FragmentManager fm) {
			super(fm);
		}

		@Override
		public Fragment getItem(int position) {
			// getItem is called to instantiate the fragment for the given page.
			// Return a PlaceholderFragment (defined as a static inner class below).
			if (position == 0) {
				mcf = new ConfigureFragment();
				return mcf;
			} else if (position == 1) {
				mlf = new LogFragment();
				return mlf;
			} else if (position == 2) {
				mlf2 = new LogFragment2();
				return mlf2;
			}
			return PlaceholderFragment.newInstance(position + 1);
		}

		@Override
		public int getCount() {
			// Show 3 total pages.
			return 3;
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
		 * The fragment argument representing the section number for this fragment.
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
		public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
			View rootView = inflater.inflate(R.layout.fragment_main, container, false);
			return rootView;
		}
	}

	public class ConfigureFragment extends Fragment {
		public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container,
				@Nullable Bundle savedInstanceState) {

			View view = inflater.inflate(R.layout.sysconfigure, container, false);
			final Button saveBtn = (Button) view.findViewById(R.id.savebtn);
			final EditText ispText = (EditText) view.findViewById(R.id.ispEdit);
			final EditText webipText = (EditText) view.findViewById(R.id.webiptext);
			try {
				File file = new File(PATH, SYSCONF);
				if(!file.exists())
					file.createNewFile();
				FileReader reader = new FileReader(file);
				
				BufferedReader br = new BufferedReader(reader);
				String s;
				String[] sarray;
				if (br != null) {
					while (null != (s = br.readLine())) {
						sarray = s.split("\"");
						for (int i = 0; i < sarray.length; i++) {
							if (sarray[i].equals("isp"))
								ispText.setText(sarray[3]);
							if (sarray[i].equals("webip"))
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
			saveBtn.setOnClickListener(new View.OnClickListener() {

				@Override
				public void onClick(View v) {
					// TODO Auto-generated method stub
					FileWriter writer;
					try {
						File file = new File(PATH, SYSCONF);
						if(!file.exists())
							file.createNewFile();
						writer = new FileWriter(file);
						if (writer != null) {
							writer.write("{	\n\"misc\":	\"/dev/bfx");
							writer.write("\",\n\"isp\":	\"");
							writer.write(ispText.getText().toString());
							writer.write("\",\n\"webip\":    \"");
							writer.write(webipText.getText().toString());
							writer.write("\",\n\"auditnelk\":	30,\n" + "\"msgnelk\":	31,\n" + "\"polltime\":	10,\n"
									+ "\"regport\":	11901,\n" + "\"auditport\":	11903,\n"
									+ "\"auditmax\":	104857600,\n" + "\"encrypt\":	0,\n" + "\"record\":   0\n}");
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
		public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container,
				@Nullable Bundle savedInstanceState) {
			View view = inflater.inflate(R.layout.log, container, false);
			TextView tv = (TextView) view.findViewById(R.id.InfologView);
			StringBuffer sb = new StringBuffer();
			try {
				FileReader fr = new FileReader(PATH + INFOLOG);
				BufferedReader br = new BufferedReader(fr);
				String s;
				while (null != (s = br.readLine()))
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

	public class LogFragment2 extends Fragment {
		public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container,
				@Nullable Bundle savedInstanceState) {
			View view = inflater.inflate(R.layout.log2, container, false);
			TextView tv = (TextView) view.findViewById(R.id.DebuglogView);
			StringBuffer sb = new StringBuffer();
			try {
				FileReader fr = new FileReader(PATH + DEBUGLOG);
				BufferedReader br = new BufferedReader(fr);
				String s;
				while (null != (s = br.readLine()))
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
