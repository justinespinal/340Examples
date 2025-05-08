import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;

public class Scheduling{

    public static ArrayList<HashMap<String, Integer>> loadProcesses(String f){
        ArrayList<HashMap<String, Integer>> processes = new ArrayList<>();
        try {
            File file = new File(f);
            Scanner scanner = new Scanner(file);
            while (scanner.hasNextLine()) {
                String line = scanner.nextLine();
                String[] values = line.split(",");
                HashMap<String, Integer> parameters = new HashMap<>();
                parameters.put("ID", Integer.parseInt(values[0]));
                parameters.put("AT", Integer.parseInt(values[1]));
                parameters.put("BT", Integer.parseInt(values[2]));
                parameters.put("P", Integer.parseInt(values[3]));
                parameters.put("FT", 0);
                parameters.put("TA", 0);
                processes.add(parameters);
            }
            scanner.close();
        } catch (FileNotFoundException e) {
            System.out.println("File not found: " + e.getMessage());
        }
        return processes;
    }

    public static void main(String args[]){
        System.out.println("Running Scheduling Algorithms!\n===================");
        String algorithm = args[0];
        String file = args[1];

        if(algorithm.equals("FCFS")){
            ArrayList<HashMap<String, Integer>> processesFCFS = loadProcesses(file);
            FCFS fcfs = new FCFS(processesFCFS);
            fcfs.run();
        }

        if(algorithm.equals("SRTF")){
            ArrayList<HashMap<String, Integer>> processesSRTF = loadProcesses(file);
            SRTF srtf = new SRTF(processesSRTF);
            srtf.run();
        }

        if(algorithm.equals("PP")){
            ArrayList<HashMap<String, Integer>> processesPP = loadProcesses(file);
            PP pp = new PP(processesPP);
            pp.run();
        }

        if(algorithm.equals("RR")){
            ArrayList<HashMap<String, Integer>> processesRR = loadProcesses(file);
            int TQ = Integer.parseInt(args[2]);
            RR rr = new RR(processesRR, TQ);
            rr.run();
        }
    }
}