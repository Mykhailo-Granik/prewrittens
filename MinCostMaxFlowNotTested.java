import java.io.*;
import java.util.*;

class Solution {

    public static final int INFINITY = 1000000000;

    static class Edge {
        private int to;
        private int capacity;
        private int cost;
        private Edge backwardEdge;

        public Edge(int to, int capacity, int cost) {
            this.to = to;
            this.capacity = capacity;
            this.cost = cost;
        }

        public void setBackwardEdge(Edge backwardEdge) {
            this.backwardEdge = backwardEdge;
        }
    }

    static class MinCostMaxFlow {

        public static class MinCostMaxFlowResult {
            private int flow;
            private int cost;

            public MinCostMaxFlowResult(int flow, int cost) {
                this.flow = flow;
                this.cost = cost;
            }

            public int getFlow() {
                return flow;
            }

            public int getCost() {
                return cost;
            }
        }

        private List<List<Edge>> adjacencyLists;

        public MinCostMaxFlow(int graphSize) {
            adjacencyLists = new ArrayList<>();
            for (int i = 0; i < graphSize; ++i) {
                adjacencyLists.add(new ArrayList<>());
            }
        }

        void addEdge(int from, int to, int capacity, int cost)
        {
            Edge forwardEdge = new Edge(to, capacity, cost);
            adjacencyLists.get(from).add(forwardEdge);
            Edge backwardEdge = new Edge(from, 0, -cost);
            adjacencyLists.get(to).add(backwardEdge);
            forwardEdge.setBackwardEdge(backwardEdge);
            backwardEdge.setBackwardEdge(forwardEdge);
        }

        private List<Integer> zeroList(int size) {
            List<Integer> res = new ArrayList<>();
            for (int i = 0; i < size; ++i) {
                res.add(0);
            }
            return res;
        }

        public MinCostMaxFlowResult calculateFlow(int s, int t)
        {
            int N = adjacencyLists.size();
            int res = 0;
            int tcap = 0;
            List<Integer> inq = zeroList(N);
            List<Edge> how = new ArrayList<>();
            for (int i = 0; i < N; ++i) {
                how.add(null);
            }
            List<Integer> from = zeroList(N);
            List<Integer> d = zeroList(N);
            while (true)
            {
                List<Integer> que = new ArrayList<>();
                for (int i = 0; i < inq.size(); ++i)
                {
                    inq.set(i, 0);
                    d.set(i, INFINITY);
                }
                inq.set(s, 1);
                d.set(s, 0);
                que.add(s);
                int l = 0;
                while (l < que.size())
                {
                    int v = que.get(l);
                    l++;
                    inq.set(v, 0);
                    for (int i = 0; i < adjacencyLists.get(v).size(); ++i) {
                        Edge edge = adjacencyLists.get(v).get(i);
                        int ver = edge.to;
                        if ((edge.capacity>0) && (d.get(v) + edge.cost<d.get(ver)))
                        {
                            d.set(ver, d.get(v) + edge.cost);
                            if (inq.get(ver) == 0)
                            {
                                inq.set(ver, 1);
                                que.add(ver);
                            }
                            from.set(ver, v);
                            how.set(ver, edge);
                        }
                    }
                }
                if (d.get(t) == INFINITY) {
                    break;
                }
                int now = INFINITY + INFINITY;
                int v = t;
                while (v != s)
                {
                    now = Math.min(now, how.get(v).capacity);
                    v = from.get(v);
                }
                v = t;
                tcap += now;
                while (v != s)
                {
                    res += now * how.get(v).cost;
                    how.get(v).capacity -= now;
                    how.get(v).backwardEdge.capacity += now;
                    v = from.get(v);
                }
            }
            return new MinCostMaxFlowResult(tcap,res);
        }

    }

    static class Tree {

        private List<Integer> ore;
        private int n;
        private List<List<Integer>> adjacencyList;
        private MinCostMaxFlow minCostMaxFlow;


        public Tree(int n, List<List<Integer>> edges, List<Integer> ore, int k) {
            this.n = n;
            this.ore = ore;
            adjacencyList = new ArrayList<>();
            for (int i = 0; i < n; ++i) {
                adjacencyList.add(new ArrayList<>());
            }
            for (List<Integer> edge : edges) {
                adjacencyList.get(edge.get(0)).add(edge.get(1));
                adjacencyList.get(edge.get(1)).add(edge.get(0));
            }
            minCostMaxFlow = new MinCostMaxFlow(2 * n + 2);
            for (int i = 0; i < n; ++i) {
                minCostMaxFlow.addEdge(i * 2, i * 2 + 1, 1, -ore.get(i));
                minCostMaxFlow.addEdge(i * 2, i * 2 + 1, INFINITY, 0);
            }
            for (int i = 0; i < n; ++i) {
                minCostMaxFlow.addEdge(i * 2 + 1, 2* n, INFINITY, 0);
                minCostMaxFlow.addEdge(2* n + 1, i * 2, INFINITY, 0);
            }
            for (List<Integer> edge : edges) {
                int v1 = edge.get(0);
                int v2 = edge.get(1);
                minCostMaxFlow.addEdge(v1 * 2 + 1, v2 * 2, 1, 0);
                minCostMaxFlow.addEdge(v2 * 2 + 1, v1 * 2, 1, 0);
            }
            minCostMaxFlow.addEdge(2 * n, 2 * n + 1, k, 0);
        }

        public int maximizeOre(int vertex, int cameFrom, int childNumber, int connectionToPreviousChildCost, int tookChildrenBefore, int k) {
            if (childNumber == adjacencyList.get(vertex).size()) {
                return ore.get(vertex);
            }
            int nextVertex = adjacencyList.get(vertex).get(childNumber);
            if (nextVertex == cameFrom) {
                return maximizeOre(vertex, cameFrom, childNumber + 1, connectionToPreviousChildCost, tookChildrenBefore, k);
            }
            int childKUpperLimit = k - connectionToPreviousChildCost;
            if ((vertex == 0) && (connectionToPreviousChildCost == 0)) {
                childKUpperLimit--;
            }
            int res = maximizeOre(vertex, cameFrom, childNumber + 1, connectionToPreviousChildCost, tookChildrenBefore, k);
            for (int childK = 0; childK <= childKUpperLimit; ++childK) {
                int leftK = k - childK - connectionToPreviousChildCost;
                if (leftK < 0) {
                    continue;
                }
                res = Math.max(res,
                        maximizeOre(nextVertex, vertex, 0, 0, 0, childK) +
                                maximizeOre(vertex, cameFrom, childNumber + 1, tookChildrenBefore ^ 1, 1, leftK));
            }
            return res;
        }

        public List<Integer> deepestPath(int vertex, int cameFrom) {
            List<Integer> result = new ArrayList<>();
            for (int nextVertex : adjacencyList.get(vertex)) {
                if (nextVertex == cameFrom) {
                    continue;
                }
                List<Integer> childDeepestPath = deepestPath(nextVertex, vertex);
                result.add(childDeepestPath.get(0) + ore.get(vertex));
            }
            if (result.isEmpty()) {
                result.add(ore.get(vertex));
            }
            result.sort(Collections.reverseOrder());
            return result;
        }

    }

    public static void main(String[] args) throws FileNotFoundException {
        InputReader in = new InputReader(new FileInputStream("C:\\Users\\Fcdkb\\Documents\\Sources\\Contests\\in.txt"));
//        PrintWriter out = new PrintWriter(new FileOutputStream("C:\\Users\\Fcdkb\\Documents\\Sources\\Contests\\out.txt"));
        PrintWriter out = new PrintWriter(System.out);
        int tests = in.nextInt();
        for (int testNumber = 1; testNumber <= tests; ++testNumber) {
            int n = in.nextInt();
            int k = in.nextInt();
            List<Integer> ore = new ArrayList<>();
            for (int i = 0; i < n; ++i) {
                ore.add(in.nextInt());
            }
            List<List<Integer>> edges = new ArrayList<>();
            for (int i = 0; i < n - 1; ++i) {
                int vertex1 = in.nextInt() - 1;
                int vertex2 = in.nextInt() - 1;
                edges.add(List.of(vertex1, vertex2));
            }
            Tree tree = new Tree(n, edges, ore, k);
            MinCostMaxFlow.MinCostMaxFlowResult minCostMaxFlowResult = tree.minCostMaxFlow.calculateFlow(0, 1);
            int res = minCostMaxFlowResult.cost;
            out.printf("Case #%d: %d\n", testNumber, -res);
            out.flush();
        }
        out.close();
    }

    static class InputReader {
        public BufferedReader reader;
        public StringTokenizer tokenizer;

        public InputReader(InputStream stream) {
            reader = new BufferedReader(new InputStreamReader(stream), 32768);
            tokenizer = null;
        }

        public String next() {
            while (tokenizer == null || !tokenizer.hasMoreTokens()) {
                try {
                    tokenizer = new StringTokenizer(reader.readLine());
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }
            return tokenizer.nextToken();
        }

        public int nextInt() {
            return Integer.parseInt(next());
        }

        public long nextLong() {
            return Long.parseLong(next());
        }


    }

}
