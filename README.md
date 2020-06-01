# SnapTrip
SnapTrip, the final project of the Advanced Programming course, in phase 1, my focus is on the proper implementation of the database and creating the right API.

## Sections

> User-Interface

> API
>> Result

> Business-Logic

> Persistence-Layer(ORM)

> Models
>> Database
>>> User &
>>> Comment

>>> Hotel
>>>> Room

## User-Interface
It is the part that you can see what SnapTrip does and you don't have to worry about what happens in the backward
But in this phase of SnapTrip (phase 1), the User-Interface doesn't have a big role in the game and as I mentioned before, in this phase we want to implement a good API and make sure of the Backend part to does everything right.
You can use those order in the [Commands](#commands) part in the terminal to see the performance of SnapTrip.

## API
**API** is the connection between the **User-Interface** and **ORM** and **Backend**. The **API** receives the entered commands and transfers them to the **Business-Logic** layer. After all the steps, it takes the results and stores them in the [Result](#result-class) class, which I'll talk about that later.

Why did I consider the **API** layer for my project? The answer is that on **SnapTrip**, we may have a lot of clients or want to run **SnapTrip** in different environments. We can't plan for all different environments, so the **API** takes the raw output information and the [Result](#result-class) class gets the result to provide the information depending on our environment.

### Result-Class
The **Result** class is in the **API** layer, and after all the steps in the **Business-Logic** layer and the **ORM** layer, the **API** calls the class to take the result and prepare them for the interface.

## ORM
**ORM** stands for **object-relational-mapping** and in charge to simplify data for Database. **ORM** has directly in touch with the database, it means that if every part of the program wants to do anything with the data, then they have to pass data through the **ORM**


## Commands
##### the order of the commands-order doesn't matter

- Make executable file
```bash
make
```
- Run the program with init data
```bash
./utrip Data/Hotels.csv
```
- Signup
```bash
POST signup ? username <username> email <email> password <password>
```
- Login
```bash
POST login ? email <email> password <password>
```

- Logout
```bash
POST logout
```
- Increase your credit
```bash
POST wallet ? amount <amount>
```
- See your _count_ account turnover
```bash
GET wallet ? count <count>
```
- See all of the hotels. if you added some filters, first applies the filters and then shows you the filtered hotels
```bash
GET hotels
```
- Enter a hotel id and see the hotel's informations
```bash
GET hotels ? id <id>
```
- Add Filter
    - This filter only shows hotels in the city specified in the command
    ```bash
    POST filters ? city <city>
    ```
    - This filter only approved hotels whose stars are specified in the range (in the interval [min, max]) (at least: 1 and at most: 5)
    ```bash
    POST filters ? min_star <min_star> max_star <max_star>
    ```
    - This filter only approved hotels whose average price are specified in the range (in the interval [min, max])
    ```bash
    POST filters ? min_price <min_price> max_price <max_price>
    ```
    - This filter only approved hotels whose have specific rooms in the specified range (in the interval [min, max]) (room type are: standard, deluxe, luxury, premium) (check_in at least: 1 night, check_out at most: 30 night)
    ```bash
    POST filters ? type <room_type> qunatity <quantity> check_in <check_in> check_out <check_out>
    ```
- Delete all Filters
```bash
DELETE filters
```
- Book up rooms in the specified hotel
```bash
POST reserves ? hotel <hotel_id> type <room_type> quantity <quantity> check_in <check_in> check_out <check_out>
```
- View the information on your bookings
```bash
GET reserves
```
- Cancel a reservation, Just enter the reservation id
```bash
DELETE reserves ? id <reservation_id>
```
- Enter a hotel's id and post a comment for that hotel
```bash
POST comments ? hotel <hotel_id> comment <comment>
```
- Hotels can be graded in 6 ways
    - location, cleanliness, staff, facilities, value_for_money, overall_rating
    - Points must be a number from 1 to 5
    - If a user points to a hotel twice, new points will be replaced
```bash
POST ratings ? hotel <hotel_id> location <location> cleanliness <cleanliness> staff <staff> facilities <facilities> value_for_money <value_for_money> overall_rating <overall_rating>
```
- View the comments on a hotel, just enter hotel id
    - The first comment is the newest comment
```bash
GET comments ? hotel <hotel_id>
```
- View the average score of a hotel
```bash
GET ratings ? hotel <hotel_id>
```



