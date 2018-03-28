from flask import Flask, flash, redirect, render_template, request, session, jsonify
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash
from cs50 import SQL

from helpers import *

# Configure application
app = Flask(__name__)

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd
app.jinja_env.filters["transaction_type"] = transaction_type

# # Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
app.config['JSONIFY_PRETTYPRINT_REGULAR'] = False
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")



@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # retrieve all stocks owned by this owner from databases
    result_set = db.execute("SELECT symbol, shares FROM assets WHERE userid = :userid", userid=session.get("user_id"))
    if result_set == None:
        return apology("Error querying database")
    # complete the stock list with its current price and full name information
    stocks = []
    for row in result_set:
        symbol = row['symbol']
        shares = row['shares']

        stock_dict = lookup(symbol)
        if not stock_dict:
            return apology("Oops, one of the stock in your portfolio does not exist")
        stocks.append({
            'symbol': symbol,
            'name': stock_dict.get("name", ""),
            'shares': shares,
            'price':  stock_dict.get("price", 0),
            'total': stock_dict.get("price", 0) * shares, # the current total value of holdings on this stock
        })
    # compute the cash user has now
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session.get("user_id"))[0]['cash']
    # compute the total wealth
    total_wealth = cash + sum([s['total'] for s in stocks])
    return render_template("index.html", stocks=stocks, cash=cash, total_wealth=total_wealth)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    elif request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol or not shares:
            return apology("Invalid symbol or shares")
    
        
        try:
            shares = int(shares)
            if shares <= 0:
                raise AssertionError
        except:
            return apology("Invalid number of shares")

        # check current symbol quote
        stock = lookup(symbol)
        if not stock:
            return apology("Cannot find this stock {}".format(symbol))
        
        # check user's balance enough to buy shares of stock 
        u_bal = db.execute("SELECT cash FROM users WHERE id = :id", id=session.get("user_id"))[0]['cash']
        if u_bal < shares*stock['price']:
            return apology("Sorry your balance is not enough")
        
        # deduct cash from user account
        db.execute("UPDATE users SET cash = cash - :pay WHERE id = :id", pay=shares*stock['price'], id=session.get("user_id"))

        # Then we are ready to commit the purchase, record this purchase in asset book
        # if user doesn't have this stock, make one
        if not db.execute("SELECT * from assets WHERE userid = :userid AND symbol = :symbol", userid=session.get("user_id"), 
                symbol = stock['symbol']):
            db.execute("INSERT INTO assets (userid, symbol, shares) VALUES (:userid, :symbol, :shares)",
                userid=session.get("user_id"), symbol=stock['symbol'], shares=shares)
        else:
        # if user already has this stock, update it
            db.execute("UPDATE assets SET shares = shares + :more WHERE userid = :userid AND symbol = :symbol",
                more=shares, userid=session.get("user_id"), symbol=stock['symbol'])
        
        # record this purchase in transaction book
        # boolean has to be stored as integer
        db.execute("INSERT INTO transactions (userid, symbol, isBuy, thenPrice, shares, dt) VALUES (\
            :userid, :symbol, :isBuy, :thenPrice, :shares, DATETIME())", userid=session.get("user_id"), 
            symbol=stock['symbol'], isBuy=1, thenPrice=stock['price'], shares=shares)

        flash("Successfully purchase {} stock {} at ${:.2f} each share!".format(shares, stock['symbol'], stock['price']))
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    records = db.execute("SELECT * FROM transactions WHERE userid = :user_id ORDER BY dt DESC",
    user_id=session.get("user_id"))
    return render_template("history.html", records=records)



@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Symbol should not empty or symbol is not in the form")
        stock = lookup(symbol)
        if not stock:
            return apology("Invalid symbol")
        return render_template("quoted.html", stock=stock)
    
    


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    else:
        # check username is in the form
        if not request.form.get("username"):
            return apology("Username is empty")
        # check username is not occupied
        if len(db.execute("SELECT username FROM users WHERE username = :username", username=request.form.get("username"))) != 0:
            return apology("Username already exists")
        
        # check two password are the same
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if not password or not confirmation:
            return apology("Passwords don't exist or invalid")
        if password != confirmation:
            return apology("Confirm password doesn't match")
        
        hashed_password = generate_password_hash(password)
        # insert hashed password into database
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=request.form.get("username"),
            hash=hashed_password)
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        result_set = db.execute("SELECT symbol, shares FROM assets WHERE userid = :user_id", user_id=session.get("user_id"))
        stock_dicts = [dict(symbol=r['symbol'], shares=r['shares']) for r in result_set]
        return render_template("sell.html", stock_dicts=stock_dicts)
    elif request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol or not shares:
            return apology("Invalid symbol or shares")
        
        try:
            shares = int(shares)
        except:
            return apology("Invalid shares, must be an integer")

        
        stock = lookup(symbol)
        if not stock:
            return apology("Cannot find this stock {}".format(symbol))
        # check if the user has this stock or that many to sell
        
        ownership = db.execute("SELECT * FROM assets WHERE userid = :user_id AND symbol = :symbol", 
            user_id=session.get("user_id"), symbol=stock['symbol'])

        if not ownership or ownership[0]['shares'] < shares:
            return apology("You do not own that stock or that many shares")
        
        # add cash to user account
        db.execute("UPDATE users SET cash = cash + :receive WHERE id = :id", receive=shares*stock['price'], id=session.get("user_id"))

        # Then we are ready to commit the purchase, record this purchase in asset book
        # if user clear all his shares on this stock, delete the row, or just minus
        if (ownership[0]['shares'] - shares) == 0:
            db.execute("DELETE FROM assets WHERE userid = :userid AND symbol = :symbol", 
                userid=session.get("user_id"), symbol=stock['symbol'])
        else:
            db.execute("UPDATE assets SET shares = shares - :less WHERE userid = :userid AND symbol = :symbol",
                less=shares, userid=session.get("user_id"), symbol=stock['symbol'])
        
        # record this purchase in transaction book
        # boolean has to be stored as integer
        db.execute("INSERT INTO transactions (userid, symbol, isBuy, thenPrice, shares, dt) VALUES (\
            :userid, :symbol, :isBuy, :thenPrice, :shares, DATETIME())", userid=session.get("user_id"), 
            symbol=stock['symbol'], isBuy=0, thenPrice=stock['price'], shares=shares)

        flash("Successfully sell {} stock {} at ${:.2f} each share!".format(shares, stock['symbol'], stock['price']))
        return redirect("/")

@app.route("/username_is_available")
def username_is_available():
    if not request.args.get("username"):
        return jsonify(response=False)
    else:
        if not db.execute("SELECT username FROM users WHERE username = :username", username=request.args.get("username")):
            return jsonify(response=True)
        else:
            return jsonify(response=False)



def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)

