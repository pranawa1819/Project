package practice.project;

import java.sql.*;
import java.util.Scanner;

public class My {
    private static Connection conn;
    private static Scanner scanner = new Scanner(System.in);

    public static String USERNAME = "root";
    public static String PASSWORD = "pranawa123";
    public static String URL = "jdbc:mysql://localhost:3306/pranawa";

    public static void main(String[] args) throws Exception {
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            // Connect to MySQL database
            conn = DriverManager.getConnection(URL, USERNAME, PASSWORD);
            System.out.println("Connected to the database");

            // Create tables if they don't exist
            createTables();

            String[] roomTypes = {"Single", "Double", "Suite"};
            double[] roomPrices = {2500.0, 4000.0, 10000.0};

            // Insert room types and prices into the database
            insertRoomTypes(roomTypes, roomPrices);

            while (true) {
                System.out.println("┌───────────────────────────────────────────────────────────────────────┐");
                System.out.println("|             WELCOME TO HOTEL MANAGEMENT SYSTEM                        |");
                System.out.println("└───────────────────────────────────────────────────────────────────────┘");
                System.out.println("|   1. Check-In                                                         |");
                System.out.println("|   2. Check-Out                                                        |");
                System.out.println("|   3. View Occupied Room                                               |");
                System.out.println("|   4. View Check-Out Room and Guest                                    |");
                System.out.println("|   5. Exit                                                             |");
                System.out.println("└───────────────────────────────────────────────────────────────────────┘");
                System.out.print("\nEnter your choice: ");

                int choice = scanner.nextInt();
                scanner.nextLine(); // consume newline character

                switch (choice) {
                    case 1:
                        Check_In();
                        break;
                    case 2:
                        Check_Out();
                        break;
                    case 3:
                        View_Occupancy();
                        break;
                    case 4:
                        View_CheckOuts();
                        break;
                    case 5:
                        conn.close();
                        System.exit(0);
                        break;
                    default:
                        System.out.println("\nInvalid choice. Please try again.");
                        break;
                }
            }
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    private static void createTables() {
        try (Statement stmt = conn.createStatement()) {
            // Create "rooms" table
            String createRoomsTable = "CREATE TABLE IF NOT EXISTS rooms ("
                    + "room_number INT PRIMARY KEY,"
                    + "room_type VARCHAR(20),"
                    + "room_price DOUBLE,"
                    + "guest_name VARCHAR(50),"
                    + "phone_number VARCHAR(20),"
                    + "check_in_date DATE)";
            stmt.executeUpdate(createRoomsTable);

            // Create "checkouts" table
            String createCheckoutsTable = "CREATE TABLE IF NOT EXISTS checkouts ("
                    + "room_number INT PRIMARY KEY,"
                    + "guest_name VARCHAR(50),"
                    + "check_in_date DATE,"
                    + "check_out_date DATE,"
                    + "room_type VARCHAR(20))";
            stmt.executeUpdate(createCheckoutsTable);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    private static void insertRoomTypes(String[] roomTypes, double[] roomPrices) throws SQLException {
        String insertRoomQuery = "INSERT INTO rooms (room_number, room_type, room_price) VALUES (?, ?, ?) ON DUPLICATE KEY UPDATE room_type=VALUES(room_type), room_price=VALUES(room_price)";
        try (PreparedStatement pstmt = conn.prepareStatement(insertRoomQuery)) {
            for (int i = 0; i < 10; i++) {
                pstmt.setInt(1, i + 1);
                pstmt.setString(2, roomTypes[i % 3]);
                pstmt.setDouble(3, roomPrices[i % 3]);
                pstmt.executeUpdate();
            }
        }
    }

    private static void Check_In() throws SQLException {
        System.out.println("\n -----------------------------------------");
        System.out.println(" |      Room Types and Prices:            |");
        System.out.println(" ------------------------------------------");
        System.out.println(" |    1. Single - 2500.00                  |");
        System.out.println(" |    2. Double - 4000.00                  |");
        System.out.println(" |    3. Suite - 10000.00                  |");
        System.out.println(" ------------------------------------------");

        System.out.print("\nEnter the number of rooms you want to book: ");
        int numRooms = scanner.nextInt();
        scanner.nextLine(); // consume newline character

        for (int i = 0; i < numRooms; i++) {
            System.out.print("\nEnter the preferred room type (Single/Double/Suite): ");
            String preferredType = scanner.nextLine();

            String selectRoomQuery = "SELECT room_number FROM rooms WHERE room_type = ? AND guest_name IS NULL LIMIT 1";
            try (PreparedStatement pstmt = conn.prepareStatement(selectRoomQuery)) {
                pstmt.setString(1, preferredType);
                try (ResultSet rs = pstmt.executeQuery()) {
                    if (rs.next()) {
                        int roomNumber = rs.getInt("room_number");
                        System.out.print("\nEnter guest name: ");
                        String guestName = scanner.nextLine();
                        System.out.print("\nEnter guest phone number: ");
                        String phoneNumber = scanner.nextLine();
                        System.out.print("\nEnter check-in date (YYYY-MM-DD): ");
                        String checkInDate = scanner.nextLine();

                        String updateRoomQuery = "UPDATE rooms SET guest_name = ?, phone_number = ?, check_in_date = ? WHERE room_number = ?";
                        try (PreparedStatement updateStmt = conn.prepareStatement(updateRoomQuery)) {
                            updateStmt.setString(1, guestName);
                            updateStmt.setString(2, phoneNumber);
                            updateStmt.setString(3, checkInDate);
                            updateStmt.setInt(4, roomNumber);
                            updateStmt.executeUpdate();
                        }

                        System.out.println("\nCheck-in successful for room no. " + roomNumber + " (" + preferredType + ").");
                    } else {
                        System.out.println("\nSorry, the room is not available for type " + preferredType + ". Please try another type.");
                    }
                }
            }
        }
    }

    private static void Check_Out() throws SQLException {
        System.out.print("\nEnter room number: ");
        int roomNumber = scanner.nextInt();
        scanner.nextLine(); // consume newline character

        String selectRoomQuery = "SELECT * FROM rooms WHERE room_number = ?";
        try (PreparedStatement pstmt = conn.prepareStatement(selectRoomQuery)) {
            pstmt.setInt(1, roomNumber);
            try (ResultSet rs = pstmt.executeQuery()) {
                if (rs.next()) {
                    if (rs.getString("guest_name") != null) {
                        System.out.print("\nEnter check-out date (YYYY-MM-DD): ");
                        String checkOutDate = scanner.nextLine();

                        String insertCheckoutQuery = "INSERT INTO checkouts (room_number, guest_name, check_in_date, check_out_date, room_type) VALUES (?, ?, ?, ?, ?)";
                        try (PreparedStatement insertStmt = conn.prepareStatement(insertCheckoutQuery)) {
                            insertStmt.setInt(1, roomNumber);
                            insertStmt.setString(2, rs.getString("guest_name"));
                            insertStmt.setString(3, rs.getString("check_in_date"));
                            insertStmt.setString(4, checkOutDate);
                            insertStmt.setString(5, rs.getString("room_type"));
                            insertStmt.executeUpdate();
                        }

                        String updateRoomQuery = "UPDATE rooms SET guest_name = NULL, phone_number = NULL, check_in_date = NULL WHERE room_number = ?";
                        try (PreparedStatement updateStmt = conn.prepareStatement(updateRoomQuery)) {
                            updateStmt.setInt(1, roomNumber);
                            updateStmt.executeUpdate();
                        }

                        System.out.println("\nGuest " + rs.getString("guest_name") + " has checked out of room " + roomNumber + ".");
                        System.out.println("Check-in date: " + rs.getString("check_in_date"));
                        System.out.println("Check-out date: " + checkOutDate);
                    } else {
                        System.out.println("\nSorry, this room is already vacant.");
                    }
                } else {
                    System.out.println("\nInvalid room number. Please try again.");
                }
            }
        }
    }

    private static void View_Occupancy() throws SQLException {
        System.out.println("┌────────────────────────────────────────────────────────────────────────────────┐");
        System.out.println("| Room | Type   | Price \t | Guest \t | Phone Number\t | Check-in Date |");
        System.out.println("├────────────────────────────────────────────────────────────────────────────────┤");
        String selectRoomsQuery = "SELECT * FROM rooms";
        try (PreparedStatement pstmt = conn.prepareStatement(selectRoomsQuery); ResultSet rs = pstmt.executeQuery()) {
            while (rs.next()) {
                System.out.printf("| %-4d | %-6s | %-7.2f\t | %-10s\t | %-12s\t | %-13s |%n",
                        rs.getInt("room_number"),
                        rs.getString("room_type"),
                        rs.getDouble("room_price"),
                        rs.getString("guest_name"),
                        rs.getString("phone_number"),
                        rs.getString("check_in_date"));
            }
        }
        System.out.println("└────────────────────────────────────────────────────────────────────────────────┘");
    }

    private static void View_CheckOuts() throws SQLException {
        System.out.println("┌───────────────────────────────────────────────────────────────┐");
        System.out.println("| Room | Guest     | Check-in Date | Check-out Date | Room Type |");
        System.out.println("├───────────────────────────────────────────────────────────────┤");
        String selectCheckoutsQuery = "SELECT * FROM checkouts";
        try (PreparedStatement pstmt = conn.prepareStatement(selectCheckoutsQuery); ResultSet rs = pstmt.executeQuery()) {
            while (rs.next()) {
                System.out.printf("| %-4d | %-10s| %-13s | %-14s | %-9s |%n",
                        rs.getInt("room_number"),
                        rs.getString("guest_name"),
                        rs.getString("check_in_date"),
                        rs.getString("check_out_date"),
                        rs.getString("room_type"));
            }
        }
        System.out.println("└───────────────────────────────────────────────────────────────┘");
    }
}
