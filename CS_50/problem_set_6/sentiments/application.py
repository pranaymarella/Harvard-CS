from flask import Flask, redirect, render_template, request, url_for

import os
import sys
import helpers
from analyzer import Analyzer
import nltk

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "").lstrip("@")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets, 100 most recent
    tweets = helpers.get_user_timeline(screen_name, 100)
    
    # if tweets returns a value
    if tweets:
        # get pathway of positive and negative word files
        positives = os.path.join(sys.path[0], "positive-words.txt")
        negatives = os.path.join(sys.path[0], "negative-words.txt")
        # create analyzer object
        analyzer = Analyzer(positives, negatives)
        # create tokenizer object
        tokenizer = nltk.tokenize.TweetTokenizer()
        
        # keep track of positive, negative, and neutral
        positive, negative, neutral = 0.0, 0.0, 0.0
        
        # for every tweet within the getched tweets
        for tweet in tweets:
            #tokenize the tweet
            tokens = tokenizer.tokenize(tweet)
            # for every tweet, start score at 0
            score = 0
            
            # for every word, analyze it to get its score and add it
            for i in tokens:
                score += analyzer.analyze(i)
            
            # distingish if tweet is positive, negative, or neutral and add it
            if score > 0.0:
                positive += 1.0
            elif score < 0.0:
                negative += 1.0
            else:
                neutral += 1.0
    # tweets for user cannot be found
    else:
        return redirect(url_for("index"))

    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
