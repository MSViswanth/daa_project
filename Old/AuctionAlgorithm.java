import java.util.Arrays;

public class AuctionAlgorithm {

    public static void main(String[] args) {
        double[][] setA = {
            {3, 2},
            {4, 1},
            {8, 5}
        };
        double[][] setB = {
            {1, 2},
            {3, 6},
            {1, 5}
        };

        Object[] result = auctionAlgorithm(setA, setB);
        double[][] assignment = (double[][]) result[0];
        double totalDistance = (double) result[1];

        System.out.println("Set A:");
        printSet(setA);
        System.out.println("Set B:");
        printSet(setB);
        System.out.println("Assignment Matrix:");
        printSet(assignment);
        System.out.println("Total Distance: " + totalDistance);
    }

    public static Object[] auctionAlgorithm(double[][] setA, double[][] setB) {
        int m = setA.length;
        int n = setB.length;

        double[][] assignment = new double[m][n];
        double[] prices = new double[n];

        while (true) {
            double[][] bids = calculateBids(setA, setB, prices);

            int[] maxBidIndices = findMaxBidIndices(bids);

            double[][] newAssignment = new double[m][n];
            for (int i = 0; i < m; i++) {
                newAssignment[i][maxBidIndices[i]] = 1;
            }

            if (Arrays.deepEquals(assignment, newAssignment)) {
                break;
            }

            assignment = newAssignment;
            prices = updatePrices(prices, maxBidIndices, bids);
        }

        double totalDistance = calculateTotalDistance(setA, assignment, setB);
        
        Object[] result = new Object[2];
        result[0] = assignment;
        result[1] = totalDistance;

        return result;
    }

    public static double[][] calculateBids(double[][] setA, double[][] setB, double[] prices) {
        int m = setA.length;
        int n = setB.length;
        double[][] bids = new double[m][n];

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                bids[i][j] = calculateBid(setA[i], setB[j], prices[j]);
            }
        }

        return bids;
    }

    public static double calculateBid(double[] a, double[] b, double price) {
        double distance = 0;
        for (int i = 0; i < a.length; i++) {
            double diff = a[i] - b[i];
            distance += diff * diff;
        }
        return -distance - price;
    }

    public static int[] findMaxBidIndices(double[][] bids) {
        int m = bids.length;
        int n = bids[0].length;
        int[] maxBidIndices = new int[m];

        for (int i = 0; i < m; i++) {
            int maxIndex = 0;
            double maxBid = bids[i][0];
            for (int j = 1; j < n; j++) {
                if (bids[i][j] > maxBid) {
                    maxBid = bids[i][j];
                    maxIndex = j;
                }
            }
            maxBidIndices[i] = maxIndex;
        }

        return maxBidIndices;
    }

    public static double[] updatePrices(double[] prices, int[] maxBidIndices, double[][] bids) {
        int n = prices.length;

        for (int j = 0; j < n; j++) {
            double maxBidForJ = Double.NEGATIVE_INFINITY;
            for (int i = 0; i < maxBidIndices.length; i++) {
                if (maxBidIndices[i] == j) {
                    maxBidForJ = Math.max(maxBidForJ, bids[i][j]);
                }
            }
            prices[j] += maxBidForJ;
        }

        return prices;
    }

    public static double calculateTotalDistance(double[][] setA, double[][] assignment, double[][] setB) {
        double totalDistance = 0;

        for (int i = 0; i < setA.length; i++) {
            int assignedColumn = -1;
            for (int j = 0; j < setB.length; j++) {
                if (assignment[i][j] == 1) {
                    assignedColumn = j;
                    break;
                }
            }
            if (assignedColumn != -1) {
                totalDistance += calculateDistance(setA[i], setB[assignedColumn]);
            }
        }

        return totalDistance;
    }

    public static double calculateDistance(double[] a, double[] b) {
        double distance = 0;
        for (int i = 0; i < a.length; i++) {
            double diff = a[i] - b[i];
            distance += diff * diff;
        }
        return Math.sqrt(distance);
    }

    public static void printSet(double[][] set) {
        for (double[] row : set) {
            System.out.println(Arrays.toString(row));
        }
    }
}
