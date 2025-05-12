import java.io.File;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Queue;

public class FCFS {
    ArrayList<HashMap<String, Integer>> processes;

    public static ArrayList<HashMap<String, Integer>> cloneProcesses(ArrayList<HashMap<String, Integer>> original) {
        ArrayList<HashMap<String, Integer>> clone = new ArrayList<>();
        
        for (HashMap<String, Integer> process : original) {
            HashMap<String, Integer> processClone = new HashMap<>(process); // shallow copy of the map (good here since keys/values are immutable)
            clone.add(processClone);
        }
        
        return clone;
    }

    public FCFS(ArrayList<HashMap<String, Integer>> processes) {
        if(!processes.isEmpty())
            this.processes = cloneProcesses(processes);
        else{
            System.out.println("Error reading data.");
            System.exit(0);
        }
    }

    public void run(){
        System.out.println("Running First Come First Serve");
        int time = 0;
        int curr = 0;
        int processesCompleted = 0;
        Queue<HashMap<String, Integer>> queue = new LinkedList<>();
        while (processesCompleted < processes.size()) { 
            while(curr < processes.size() && processes.get(curr).get("AT")==time){
                queue.add(processes.get(curr));
                curr += 1;
            }
            if(!queue.isEmpty() && queue.peek().get("BT") > 0){
                int BT = queue.peek().get("BT")-1;
                queue.peek().put("BT", BT);
                if(BT == 0){
                    queue.peek().put("FT", time+1);
                    queue.peek().put("TA", time+1-queue.peek().get("AT"));
                    queue.poll();
                    processesCompleted += 1;
                }
            }
            time += 1;
        }
        print(this.processes);
    }

    public void print(ArrayList<HashMap<String, Integer>> p){
        for(HashMap<String, Integer> map: p){
            System.out.println(map.get("ID")+","+map.get("AT")+","+map.get("FT")+","+map.get("TA"));
        }
        write(p);
    }

    public void write(ArrayList<HashMap<String, Integer>> p){
        try {
            String fileName = "out2.txt";
            File file = new File(fileName);
            FileWriter writer = new FileWriter(file);

            for(int i = 0; i < p.size(); i++){
                HashMap<String, Integer> map = p.get(i);
                String content = map.get("ID")+","+map.get("AT")+","+map.get("FT")+","+map.get("TA")+"\n";
                writer.write(content);
            }
            writer.close();
        } catch (Exception e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
    }
}
