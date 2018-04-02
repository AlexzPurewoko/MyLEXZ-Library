package com.mylexz.utils;
import java.util.*;
import android.content.*;
import java.io.*;

public class NodeData
{	
	static {
		System.loadLibrary("mNDATA");
	}
	private int __id = -1;
	private String __filepath = "";
	private String __signature = "";
	private int type = 0;
	private List<String> mlist;
	private List<String> mTemp;
	private int proc = 0;
	private static final String DEFAULT_SIGNATURE = "NDataFile";
	public static final int TYPE_INT = 'i';
	public static final int TYPE_LONG = 'L';
	public static final int TYPE_DOUBLE = 'F';
	public static final int TYPE_CHAR = 'c';
	public static final int TYPE_STRING = 's';
	public static final int TYPE_BOOLEAN = 'b';
	public static final int ID_NODE = '(';
	public static final int ID_ARRAY = '>';
	public static final int ID_DATA = '<';
	public static final int MODE_PRIVATE 	= 0x000001;
	public static final int MODE_PUBLIC 	= 0x000010;
	public NodeData(Context c, String nameFile, String signature, int mode){
		File path;
		String fpath = null;
		String p = c.getPackageName();
		switch(mode){
			case MODE_PUBLIC :
				fpath = c.getExternalFilesDir(null).getPath()+"/"+nameFile;
				break;
			case MODE_PRIVATE:
				path = new File("/data/data/"+p+"/files");
				path.mkdirs();
				fpath = path.getPath()+"/"+nameFile;
		}
		__filepath = fpath;
		__signature = signature;
		mlist = mTemp = new ArrayList<String>();
		
	}
	public NodeData(String filepath, String signature){
		__filepath = filepath;
		__signature = signature;
		mlist = mTemp = new ArrayList<String>();
	}
	public NodeData(String filepath){
		__filepath = filepath;
		__signature = DEFAULT_SIGNATURE;
		mlist = mTemp = new ArrayList<String>();
	}
	// const char *nameFile, const char *signature
	public native void open();
	public native int close();
	public native void addNode(String path, String node_name);
	public native void delNode(String fullpath);
	public native void clearContents(String fullpath);
	public native String[] listContents(String fullpath);
	
	public native void addIntData(String path, String elem, int content);
	public native void addLongData(String path, String elem, long content);
	public native void addCharData(String path, String elem, char content);
	public native void addBoolData(String path, String elem, boolean content);
	public native void addStrData(String path, String elem, String content);
	public native void addDoubleData(String path, String elem, double content);
	public native int getIntData(String fullpath);
	public native boolean getBooleanData(String fullpath);
	public native long getLongData(String fullpath);
	public native double getDoubleData(String fullpath);
	public native char getCharData(String fullpath);
	public native String getStringData(String fullpath);
	public native void setContentData(String fullpath, int content);
	public native void setContentData(String fullpath, long content);
	public native void setContentData(String fullpath, double content);
	public native void setContentData(String fullpath, String content);
	public native void setContentData(String fullpath, boolean content);
	public native void setContentData(String fullpath, char content);
	
	public native boolean isAnyErrors();
	public native void deleteData(String fullpath);
	public native void clearDataValue(String fullpath);
	public native boolean isEmptyData(String fullpath);
	/*
	public native void addIntArray(String path, String name_arr, int[] content);
	public native void addStringArray(String path, String name_arr, String[] content);
	public native void addCharArray(String path, String name_arr, char[] content);
	public native void addDoubleArray(String path, String name_arr, double[] content);
	public native void addLongArray(String path, String name_arr, long[] content);
	public native void addBooleanArray(String path, String name_arr, boolean[] content);
	 public native int getOccurences(String fullpath, int data);
	 public native int getOccurences(String fullpath, long data);
	 public native int getOccurences(String fullpath, double data);
	 public native int getOccurences(String fullpath, String data);
	 public native int getOccurences(String fullpath, boolean data);
	 public native int getOccurences(String fullpath, char data);
	 
	
	public native int[] getIntArray(String fullpath);
	public String[] getStringArray(String fullpath){
		int length = getArrayLength(fullpath);
		if(length <= 0)return null;
		String[] data = new String[length];
		setReadArrayIteration(fullpath);
		try
		{
			for (int x = 0; x < length; x++)
				data[x] = (String) readNext();
		}
		catch (Exception e)
		{
			System.out.println(e.toString());
			return null;
		}
		resetArrayIteration();
		return data;

	}
	public native char[] getCharArray(String fullpath);
	public native double[] getDoubleArray(String fullpath);
	public native long[] getLongArray(String fullpath);
	public native boolean[] getBooleanArray(String fullpath);
	public native int[] getIntArrayAtPos(String fullpath, int start, int end);
	public String[] getStringArrayAtPos(String fullpath, int start, int end){
		int length = getArrayLength(fullpath);
		if(length <= 0 || start > end || end >= length)return null;
		int x, y, z;
		String[] data =new String[(end - start)+1];
		String tmp = "";
		setReadArrayIteration(fullpath);
		for( x = y = z = 0; x < length; x++){
			try
			{
				tmp = (String) readNext();
			}
			catch (Exception e)
			{}
			if( x == start){
				y = 1;
			}
			if(y == 1)data[z++] = tmp;
			if(x == end)break;
		}
		resetArrayIteration();
		return data;
	}
	
	public native char[] getCharArrayAtPos(String fullpath, int start, int end);
	public native double[] getDoubleArrayAtPos(String fullpath, int start, int end);
	public native long[] getLongArrayAtPos(String fullpath, int start, int end);
	public native boolean[] getBooleanArrayAtPos(String fullpath, int start, int end);
	public native int getArrayLength(String fullpath);
	
	
	public native void setArray(String fullpath, int[] content);
	public native void setArray(String fullpath, long[] content);
	public native void setArray(String fullpath, double[] content);
	public native void setArray(String fullpath, char[] content);
	public native void setArray(String fullpath, String[] content);
	public native void setArray(String fullpath, boolean[] content);
	public native void appendArray(String fullpath, int[] content);
	public native void appendArray(String fullpath, long[] content);
	public native void appendArray(String fullpath, double[] content);
	public native void appendArray(String fullpath, char[] content);
	public void appendArray(String fullpath, String[] content){
		int check = getArrayLength(fullpath);
		if(check <= 0)return;
		int x = 0;
		startAppendArray(fullpath);
		for(; x < content.length; x++)nextAppend(content[x]);
		try
		{
			save();
		}
		catch (Exception e)
		{}
	}
	public native void appendArray(String fullpath, boolean[] content);
	public native void setArrayAtPos(String fullpath, int[] content, int start, int end);
	public native void setArrayAtPos(String fullpath, long[] content, int start, int end);
	public native void setArrayAtPos(String fullpath, double[] content, int start, int end);
	public native void setArrayAtPos(String fullpath, String[] content, int start, int end);
	public native void setArrayAtPos(String fullpath, boolean[] content, int start, int end);
	public native void setArrayAtPos(String fullpath, char[] content, int start, int end);
	// for iteration collecting data in an array
	public native void setReadArrayIteration(String fullpath);
	public native Object readNext()throws Exception;
	public native void resetArrayIteration();
	// return true if any iterator operation
	public native boolean isLocked();
	public native final String getErrorDesc();
	// for iteration adding node
	public native void addNodeIter(String fullpath);
	public native void nextaddNode(String node_name);
	// for iteration appending data in an array
	
	public void startAppendArray(String fullpath){
		if(fullpath == null || mTemp.isEmpty() || proc != 0)return;
		int tp = getDataType(fullpath);
		if(tp == 0)return;
		type = tp;
		mTemp.add(fullpath);
		proc = 'a';
	}
	public void nextAppend(int data){
		if(type != TYPE_INT || proc != 'a')return;
		mlist.add(String.valueOf(data));
	}
	public void nextAppend(long data){
		if(type != TYPE_LONG || proc != 'a')return;
		mlist.add(String.valueOf(data));
	}
	public void nextAppend(double data){
		if(type != TYPE_DOUBLE || proc != 'a')return;
		mlist.add(String.valueOf(data));
	}
	public void nextAppend(boolean data){
		if(type != TYPE_BOOLEAN || proc != 'a')return;
		mlist.add(String.valueOf(data));
	}
	public void nextAppend(char data){
		if(type != TYPE_CHAR || proc != 'a')return;
		mlist.add(String.valueOf(data));
	}
	// for string data array isn't native, because for memory allocation error
	public void nextAppend(String data){
		if(type != TYPE_STRING || proc != 'a')return;
		mlist.add(data);
	}
	// for iteration adding array
	public void startAddArray(String path, String name, int type){
		if(type != 0 || mlist.isEmpty() || proc != 0)return;
		mTemp.add(path);
		mTemp.add(name);
		this.type = type;
		proc = 'b';
	}
	public void nextAddArray(int data){
		if(type != TYPE_INT || proc != 'b')return;
		mlist.add(String.valueOf(data));
	}
	public void nextAddArray(long data){
		if(type != TYPE_LONG || proc != 'b')return;
		mlist.add(String.valueOf(data));
	}
	public void nextAddArray(double data){
		if(type != TYPE_DOUBLE || proc != 'b')return;
		mlist.add(String.valueOf(data));
	}
	public void nextAddArray(boolean data){
		if(type != TYPE_BOOLEAN || proc != 'b')return;
		mlist.add(String.valueOf(data));
	}
	public void nextAddArray(char data){
		if(type != TYPE_CHAR || proc != 'b')return;
		mlist.add(String.valueOf(data));
	}
	public void nextAddArray(String data){
		if(type != TYPE_STRING || proc != 'b')return;
		mlist.add(data);
	}

	public void save() throws Exception{
		switch(proc){
			case 'a':
			{
				switch(type){
					case TYPE_INT:
					{
						int[] data = new int[mlist.size()];
						for(int x = 0; x < mlist.size(); x++)
							data[x] = Integer.parseInt(mlist.get(x));
						appendArray(mTemp.get(0), data);
						mTemp.clear();
						mlist.clear();
						
					}
					break;
					case TYPE_STRING:
					{
						String[] mArr = (String[])mlist.toArray();
						appendArray(mTemp.get(0), mArr);
						mTemp.clear();
						mlist.clear();
						
					}
					break;
					case TYPE_BOOLEAN:
					{
						boolean[] data = new boolean[mlist.size()];
						for(int x = 0; x < mlist.size(); x++)
							data[x] = Boolean.parseBoolean(mlist.get(x));
						appendArray(mTemp.get(0), data);
						mTemp.clear();
						mlist.clear();
						
					}
					break;
					case TYPE_DOUBLE:
					{
						double[] data = new double[mlist.size()];
						for(int x = 0; x < mlist.size(); x++)
							data[x] = Double.parseDouble(mlist.get(x));
						appendArray(mTemp.get(0), data);
						mTemp.clear();
						mlist.clear();
						
					}
					break;
					case TYPE_CHAR:
					{
						char[] data = new char[mlist.size()];
						for(int x = 0; x < mlist.size(); x++)
							data[x] =(mlist.get(x)).charAt(0);
						appendArray(mTemp.get(0), data);
						mTemp.clear();
						mlist.clear();
						
					}
					break;
					case TYPE_LONG:
					{
						long[] data = new long[mlist.size()];
						for(int x = 0; x < mlist.size(); x++)
							data[x] = Long.parseLong(mlist.get(x));
						appendArray(mTemp.get(0), data);
						mTemp.clear();
						mlist.clear();
						
					}
					break;
					default :
					{
						mTemp.clear();
						mlist.clear();
						type = 0;
						proc = 0;
						throw new Exception("Unknown data types!");
					}
				}
			}
			break;
			case 'b':
			{
				switch(type){
					case TYPE_INT:
					{
						int[] data = new int[mlist.size()];
						for(int x = 0; x < mlist.size(); x++)
							data[x] = Integer.parseInt(mlist.get(x));
						addIntArray(mTemp.get(0), mTemp.get(1), data);
						mTemp.clear();
						mlist.clear();
					}
					break;
					case TYPE_STRING:
					{
						String[] mArr = (String[])mlist.toArray();
						addStringArray(mTemp.get(0), mTemp.get(1), mArr);
						mTemp.clear();
						mlist.clear();
						
					}
					break;
					case TYPE_BOOLEAN:
					{
						boolean[] data = new boolean[mlist.size()];
						for(int x = 0; x < mlist.size(); x++)
							data[x] = Boolean.parseBoolean(mlist.get(x));
						addBooleanArray(mTemp.get(0), mTemp.get(1), data);
						mTemp.clear();
						mlist.clear();
						
					}
					break;
					case TYPE_DOUBLE:
					{
						double[] data = new double[mlist.size()];
						for(int x = 0; x < mlist.size(); x++)
							data[x] = Double.parseDouble(mlist.get(x));
						addDoubleArray(mTemp.get(0), mTemp.get(1), data);
						mTemp.clear();
						mlist.clear();
						
					}
					break;
					case TYPE_CHAR:
					{
						char[] data = new char[mlist.size()];
						for(int x = 0; x < mlist.size(); x++)
							data[x] =(mlist.get(x)).charAt(0);
						addCharArray(mTemp.get(0), mTemp.get(1), data);
						mTemp.clear();
						mlist.clear();
						
					}
					break;
					case TYPE_LONG:
					{
						long[] data = new long[mlist.size()];
						for(int x = 0; x < mlist.size(); x++)
							data[x] = Long.parseLong(mlist.get(x));
						addLongArray(mTemp.get(0), mTemp.get(1), data);
						mTemp.clear();
						mlist.clear();
						
					}
					break;
					default :
						throw new Exception("Unknown data types!");
				}
			}
			break;
			case 'c':
				saveIteration();
				break;
			default:
			{
				proc = type = 0;
				mlist.clear();
				mTemp.clear();
				throw new Exception("the data file is closed. make sure you call method open()");
			}
		}
		proc = type = 0;
		return;
	}
	private native void saveIteration();
	public native int getDataType(String fullpath);*/
}
