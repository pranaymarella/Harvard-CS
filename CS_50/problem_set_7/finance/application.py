from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    if request.method == "GET":
        # get current users portfolio
        portfolio = db.execute("SELECT * FROM portfolio WHERE id = :user_id", user_id = session.get("user_id"))
        # # get user cash
        cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id = session.get("user_id"))
        return render_template("index.html", portfolio  = portfolio, cash = cash[0]["cash"])
    return apology("TODO")

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "GET":
        return render_template("buy.html")
    elif request.method == "POST":
        symbol = lookup(request.form["symbol"])
        shares = request.form["shares"]
        user_id = session.get("user_id")
        
        if not symbol:
            return apology("Not a valid symbol")
        if (not shares.isdigit()):
            return apology("Invalid shares")
            
        cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id = user_id)
        if (float(shares) * float(symbol["price"]) > cash[0]["cash"]):
            return apology("You cannot afford this!")
        
        # Try to insert into the database
        if (db.execute("INSERT INTO portfolio (id, symbol, name, shares, price) VALUES (:user_id, :symbol, :name, :shares, :price)",
            user_id = user_id,
            symbol = symbol["symbol"],
            name = symbol["name"],
            shares = int(shares),
            price = float(symbol["price"]))):
                #update total
                db.execute("UPDATE portfolio SET total = total + :amount WHERE id = :user_id AND symbol = :symbol",
                user_id = user_id,
                symbol = symbol["symbol"],
                amount = float(shares) * float(symbol["price"]))
                #update history
                db.execute("INSERT INTO history (user_id, symbol, shares, price) VALUES (:user_id, :symbol, :shares, :price)", user_id = user_id, symbol = symbol["symbol"], shares = int(shares), price = float(symbol["price"]))
                #update cash
                db.execute("UPDATE users SET cash = cash - :amount WHERE id = :user_id", user_id = user_id, amount = float(symbol["price"]) * float(shares))
                return redirect(url_for('index'))
        # otherwise, update previous entry
        elif (db.execute("UPDATE portfolio SET shares = shares + :shares WHERE symbol = :symbol AND id = :user_id",
              user_id = user_id,
              symbol = symbol["symbol"],
              shares = int(shares))):
                  # update total
                  db.execute("UPDATE portfolio SET total = total + :amount WHERE id = :user_id AND symbol = :symbol",
                             user_id = user_id,
                             symbol = symbol["symbol"],
                             amount = float(shares) * float(symbol["price"]))
                  # update history
                  db.execute("INSERT INTO history (user_id, symbol, shares, price) VALUES (:user_id, :symbol, :shares, :price)", user_id = user_id, symbol = symbol["symbol"], shares = int(shares), price = float(symbol["price"]))
                  # update cash
                  db.execute("UPDATE users SET cash = cash - :amount WHERE id = :user_id", user_id = user_id, amount = float(symbol["price"]) * float(shares))
                  return redirect(url_for('index'))
        
@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    if request.method == "GET":
        history = db.execute("SELECT * FROM history WHERE user_id = :user_id ORDER BY datetime DESC", user_id = session.get("user_id"))
        return render_template("history.html", history = history)
    return apology("TODO")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]
        
        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    elif request.method == "POST":
        # store users symbol
        symbol = lookup(request.form["symbol"])
        
        # if the user provided an invalid symbol or no symbol
        if (not symbol) or symbol == "":
            return apology("That's not a valid symbol!")
        
        # return the symbol and render page with it
        return render_template("quote.html", symbol = symbol)
    
    # SOMETHING WENT TERRIBLY WRONG
    return apology("SOMETHING WENT WRONG. GO BACK AND TRY AGAIN")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    if request.method == "GET":
        return render_template("register.html")
    elif request.method == "POST":
        username = request.form["username"]
        password = request.form["password"]
        verify_password = request.form["verify_password"]
        
        # Require that a user input a username
        if username == "":
            return apology("Needs a Username!")
        # Make sure user does not try to register with a name already in database
        elif db.execute("SELECT username FROM users WHERE username = :username;", username = username):
            return apology("That username is already taken!")
        
        # Require that a user input a password
        if password == "":
            return apology("Password cannot be blank!")
        # Make sure password and verify password are the same
        elif password != verify_password:
            return apology("Password and Valid Password must match!")
        
        # all checks passed, insert data into database
        # inserting user
        db.execute("INSERT INTO users (id, username, hash, cash) values (NULL,:username,:password, 10000);",
                    username = username,
                    password = pwd_context.encrypt(password))
        
        return redirect(url_for("login"))
    # Something terrible happened
    return apology("SOMETHING FAILED, GO BACK")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "GET":
        return render_template("sell.html")
    elif request.method == "POST":
        symbol = request.form["symbol"]
        symbol_info = lookup(symbol)
        shares = request.form["shares"]
        
        # makes sure user gives us a valid symbol
        if (not symbol) or symbol == "":
            return apology("Need a valid symbol")
        # makes sure shares is an integer
        if (not shares.isdigit()):
            return apology("Invalid shares")
        
        # We know for sure that shares is an integer now
        shares = int(shares)
        
        # get users protfolio of stocks
        portfolio = db.execute("SELECT symbol, shares, price FROM portfolio WHERE id = :user_id", user_id = session.get("user_id"))
        
        # check if passed in symbol is in user portfolio
        for stock in portfolio:
            # if the symbol is in the portfolio
            if stock["symbol"] == symbol.upper():
                # make sure user is selling shares equal to or lower than he/she owns
                if (shares <= int(stock["shares"])):
                    # remove from portfolio if no shares remaining
                    if (shares == int(stock["shares"])):
                        db.execute("DELETE FROM portfolio WHERE id = :user_id AND symbol = :symbol", 
                                    user_id = session.get("user_id"),
                                    symbol = stock["symbol"])
                    else:
                        db.execute("UPDATE portfolio SET shares = shares - :shares WHERE id = :user_id AND symbol = :symbol",
                                    shares = shares,
                                    user_id = session.get("user_id"),
                                    symbol = stock["symbol"])
                    # Update History
                    db.execute("INSERT INTO history (user_id, symbol, shares, price) values (:user_id, :symbol, :shares, :price)",
                                        user_id = session.get("user_id"),
                                        symbol = symbol.upper(),
                                        shares = -1 * shares,
                                        price = float(symbol_info["price"]))
                    # Update User Cash
                    cash = db.execute("SELECT cash from users WHERE id = :user_id", user_id = session.get("user_id"))
                    db.execute("UPDATE users SET cash = cash + :cash WHERE id = :user_id",
                                cash = float(symbol_info["price"]) * float(shares),
                                user_id = session.get("user_id"))
                    return redirect(url_for('index'))
                # user is trying to sell more than he/she has, error
                else:
                    return apology("You don't have that many shares to sell")
        
        # Stock was not in the users portfolio
        return apology("You don't own this stock")

@app.route("/change_pwd", methods=["GET", "POST"])
@login_required
def change_pwd():
    """change password"""
    if request.method == "GET":
        return render_template("change_pwd.html")
    elif request.method == "POST":
        new_password = request.form["password_new"]
        verify_password = request.form["verify_password"]
        
        if (verify_password == new_password):
            db.execute("UPDATE users SET hash = :new_password WHERE id = :user_id",
                        user_id = session.get("user_id"),
                        new_password = pwd_context.encrypt(new_password))
            return redirect(url_for('index'))
        else:
            return apology("Password's do not match")
    else:
        return apology("something went wrong, go back and try again")