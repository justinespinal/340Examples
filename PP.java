import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.PriorityQueue;

public class PP {
    
    ArrayList<HashMap<String, Integer>> processes;

    public static ArrayList<HashMap<String, Integer>> cloneProcesses(ArrayList<HashMap<String, Integer>> original) {
        ArrayList<HashMap<String, Integer>> clone = new ArrayList<>();
        
        for (HashMap<String, Integer> process : original) {
            HashMap<String, Integer> processClone = new HashMap<>(process); // shallow copy of the map (good here since keys/values are immutable)
            clone.add(processClone);
        }
        
        return clone;
    }

    public PP(ArrayList<HashMap<String, Integer>> processes){
        if(!processes.isEmpty())
            this.processes = cloneProcesses(processes);
        else{
            System.out.println("Error reading data.");
            System.exit(0);
        }
    }

    public void run(){
        System.out.println("Running Preemptive Priority");
        int time = 0;
        int curr = 0;
        int processesCompleted = 0;
        PriorityQueue<HashMap<String, Integer>> queue = new PriorityQueue<>(new PriorityComparator());
        while (processesCompleted < 5) { 
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
        System.out.println("===================");
    }
}

class PriorityComparator implements Comparator<HashMap<String, Integer>> {
    @Override
    public int compare(HashMap<String, Integer> map1, HashMap<String, Integer> map2) {
        if(Integer.compare(map1.get("P"), map2.get("P")) != 0)
            return Integer.compare(map1.get("P"), map2.get("P"));
        else
            return Integer.compare(map1.get("ID"), map2.get("ID"));
    }
}
