# SnapTrip
SnapTrip, the final project of the Advanced Programming course, in phase 1, my focus is on the proper implementation of the database and creating the right API.

## Sections

> User-Interface

> API

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
- See the information on your bookings
```bash
GET reserves
```
- Cancel a reservation, Just enter the reservation id
```bash
DELETE reserves ? id <reservation_id>
```

