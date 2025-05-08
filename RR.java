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
        int time = 0;
        int curr = 0;
        int processesCompleted = 0;
        Queue<HashMap<String, Integer>> queue = new LinkedList<>();
        while (processesCompleted < this.processes.size()) { 
            while(curr < processes.size() && processes.get(curr).get("AT")==time){
                queue.add(processes.get(curr));
                curr += 1;
            }
            if(!queue.isEmpty() && queue.peek().get("BT") > 0){
                if(currTQ == TQ){
                    HashMap<String, Integer> preempted = queue.poll();
                    queue.add(preempted);
                    this.currTQ = 0;
                    continue;
                }
                int BT = queue.peek().get("BT")-1;
                queue.peek().put("BT", BT);
                if(BT == 0){
                    HashMap<String, Integer> complete = queue.poll();
                    complete.put("FT", time+1);
                    complete.put("TA", (time+1)-complete.get("AT"));
                    processesCompleted += 1;
                    this.currTQ = this.TQ;
                }
            }
            time += 1;
            currTQ += 1;
        }
        print(this.processes);
    }

    public void print(ArrayList<HashMap<String, Integer>> p){
        for(HashMap<String, Integer> map: p){
            System.out.println(map.get("ID")+","+map.get("AT")+","+map.get("FT")+","+map.get("TA"));
        }
        System.out.println("===================");
    }
}
