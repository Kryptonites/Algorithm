import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;

public class SquareSum {

	public static void main(String[] args) {
		SquareSum sm = new SquareSum();
		int low = 1, high = 10000;
		for (int i = low; i <= high; ++i) {
			// output the solutions of n in [low, high]
			System.out.printf("n=%d, %s\n", i, sm.solve(i).toString());
		}
	}
	
	/**
	 * 
	 * @param n
	 * @return the solution of n	
	 */
	public List<Integer> solve(int n) {
		LinkedList<Integer> currList = new LinkedList<>();
		Set<List<Integer>> sols = new HashSet<>();
		// searches all the possible solutions and these solutions
		// will be stored in sols.
		search(n, currList, sols);
		int len = Integer.MAX_VALUE;
		List<Integer> sol = null;
		// find the solution which contains least numbers
		for (List<Integer> tempSol: sols) {
			if (tempSol.size() < len) {
				len = tempSol.size();
				sol = tempSol;
			}
		}
		return sol;
	}
	
	private void search(int remain, LinkedList<Integer> currList,
			Set<List<Integer>> sols) {
		if (remain == 0) { // if remain == 0, then a solution found
			List<Integer> sol = new ArrayList<>(currList);
			Collections.sort(sol);
			Collections.reverse(sol);
			sols.add(sol);
		} else if (currList.size() < 4) { // four-square theorem
			int sqrt = sqrt(remain);
			for (int i = sqrt; i >= 1; --i) {
				// iterate the square numbers that less than remain
				currList.addFirst(i * i);
				search(remain - i * i, currList, sols);
				currList.removeFirst();
			}
		}
	}
	
	private int sqrt(int n) {
		return (int) Math.floor(Math.sqrt(n));
	}
	
}
