# SnapTrip
SnapTrip is the final project of an Advanced Programming course at the University of Tehran. In the first two steps, my focus is on the proper implementation of the Database, creating the right APIs, and setting up Artificial Intelligence to increase the efficiency and performance of the program to achieve the best results.

## Layers

> User-Interface

> API
>> Result

> Business-Logic
>> Artificial Intelligence

> Persistence-Layer(ORM)

> Models
>> Database
>>> User
>>>> Filters,
>>>> ReservationCase

>>> Comment

>>> Hotel
>>>> Room

## User-Interface
It is the part that you can see what SnapTrip does and you don't have to worry about what happens in the backend,
but in this phase of SnapTrip (phase 1), the User-Interface doesn't have a big role in the game and as I mentioned before, in this phase we want to implement a good API and make sure the Backend part does everything right.
You can use the orders in the [Commands](#commands) part in the terminal to see the application of SnapTrip.

## API
**API** is the connection between the **User-Interface**, **ORM** and **Backend**. The **API** receives the entered commands and transfers them to the **Business-Logic** layer. After all the steps, it takes the results and stores them in the [Result](#result-class) class, which I'll talk about later.

Why did I consider the **API** layer for my project? The answer is that on **SnapTrip**, we may have a lot of clients or want to run **SnapTrip** in different environments. We can't plan for all different environments, so the **API** takes the raw output information and the [Result](#result-class) class gets the result to provide the information depending on our environment.

### Result-Class
The **Result** class is in the **API** layer, and after all the steps in the **Business-Logic** layer and the **ORM** layer, the **API** calls the class to take the result and prepare them for the interface.

## Business-Logic
This **SnapTrip** engine and all the logic and program steps and all the functions work in this section.
Each command is sent to this layer via the **API**, and here, the command is analyzed, and if there is an error in it, *Exceptions* are returned, otherwise, the commands are transferred to the next layer, **ORM**.
To make SnapTrip smarter, we added [AI](#artificial-intelligence) to the Business-Logic layer.


### Artificial-Intelligence
If you want to see hotels by your rating but don't weigh them manually, SnapTrip estimates your weight! On what basis does SnapTrip do this? First of all, you need to rate at least 5 hotels so that SnapTrip can estimate the right weight.

Suppose you give $x^{i}$ as points to a hotel and $y^{i}$ as your total points to that hotel. For example: 

${x^1} = [2.1, 1.6, 4.6, 2.3, 1.5]$ ,   $y^{1} = 2.58$ ,  $x^{1}_{1} = 1.6$

We now estimate the level of overall satisfaction as a function of the $x^{j}$ category scores:
$\hat{y^j} = f(x^j, w) = \frac{\sum_{i = 1}^{5} (w_{i} * x^{j}_{i})}{\sum_{i = 1}^{5} w_{i}}$

And we define the error function as follows:
$E(x^j, w, y^j) = (f(x^j, w) - y^j)^2$

Now our goal is to find all the $w_{j}$ in such a way that the **error function** becomes zero for all the values ​​of *j*.

## ORM
**ORM** stands for **Object Relational Mapping** and is responsible for creating *relational models* and *tables* using input information.
**ORM** is in direct contact with *database*, which means that if any part of the program wants to do something with the data, it must transfer the data via **ORM**.

## Commands
###### *the order of the commands-order doesn't matter*

- Make executable file
```bash
make
```
- Run the SnapTrip with init data
```bash
./utrip assets/hotels.csv assets/ratings.csv
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
- See all of the hotels. if you add some filters, first applies the filters and then shows you the filtered hotels
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
    - This filter only approved hotels whose stars are specified in the range (in the interval [min, max])
        - at least: 1 and at most: 5
    ```bash
    POST filters ? min_star <min_star> max_star <max_star>
    ```
    - This filter only approved hotels whose average price are specified in the range
        - in the interval [min, max]
    ```bash
    POST filters ? min_price <min_price> max_price <max_price>
    ```
    - This filter only approved hotels whose have specific rooms in the specified range
        - room type are: standard, deluxe, luxury, premium
        - in the interval [min, max]
        - check_in at least: 1 night, check_out at most: 30 night
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
- We now have a default price filter for filtering hotels under certain conditions.
    - If the user has not set any average price filter
    - If the user has 10 or more reservations.
    - If this filter is not disabled by the user.
```bash
POST default_price_filter ? active <true|false>
```
- With the following command, you can arrange hotels ascending or descending based on the following features.
    - property: id, name, star_rating, city, standard_room_price, deluxe_room_price, luxury_room_price, premium_room_price, average_room_price, rating_personal.
```bash
POST sort ? property <property> order <ascending|descending>
```

- With the following commands, you can set manual weights to sort hotels
```bash
POST manual_weights ? active true location <location> cleanliness <cleanliness> staff <staff> facilities <facilities> value_for_money <value_for_money>
```
```bash
POST manual_weights ? active false
```

- With the following command, you can see manual weights
```bash
GET manual_weights
```

- With the following command, you can see estimated weights
```bash
GET estimated_weights
```

‍



