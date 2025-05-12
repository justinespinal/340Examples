import java.io.File;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Queue;

public class RR {

    ArrayList<HashMap<String, Integer>> processes;
    int TQ;
    int currTQ;

    public static ArrayList<HashMap<String, Integer>> cloneProcesses(ArrayList<HashMap<String, Integer>> original) {
        ArrayList<HashMap<String, Integer>> clone = new ArrayList<>();
        
        for (HashMap<String, Integer> process : original) {
            HashMap<String, Integer> processClone = new HashMap<>(process); // shallow copy of the map (good here since keys/values are immutable)
            clone.add(processClone);
        }
        
        return clone;
    }

    public RR(ArrayList<HashMap<String, Integer>> processes, int TQ){
        if(!processes.isEmpty()){
            this.processes = cloneProcesses(processes);
            this.TQ = TQ;
            this.currTQ = TQ;
        }
        else{
            System.out.println("Error reading data.");
            System.exit(0);
        }
    }

    public void run(){
        System.out.println("Running Round Robin, TQ: " + this.TQ);
        int time = 0, curr = 0, processesCompleted = 0;
        Queue<HashMap<String, Integer>> queue = new LinkedList<>();
        
        int lastID = -1;
        this.currTQ = 0;
    
        while (processesCompleted < this.processes.size()) {
            while (curr < processes.size() && processes.get(curr).get("AT") == time) {
                queue.add(processes.get(curr++));
            }
    
            if (!queue.isEmpty()) {
                int headID = queue.peek().get("ID");
                // if it's a different process than last tick, reset its quantum
                if (headID != lastID) {
                    currTQ = 0;
                    lastID = headID;
                }
    
                // if it has already used up its slice, preempt
                if (currTQ == TQ) {
                    HashMap<String, Integer> preempted = queue.poll();
                    queue.add(preempted);
                    currTQ = 0;
                    lastID = queue.peek().get("ID");  // next one is new
                    continue;
                }
    
                // otherwise run one time-unit
                int BT = queue.peek().get("BT") - 1;
                queue.peek().put("BT", BT);
    
                // if it just finished, record and reset
                if (BT == 0) {
                    HashMap<String, Integer> complete = queue.poll();
                    complete.put("FT", time + 1);
                    complete.put("TA", (time + 1) - complete.get("AT"));
                    processesCompleted++;
                    currTQ = 0;
                    lastID = -1; // CPU will pick a fresh process next
                }
            }
    
            time++;
            currTQ++;
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
