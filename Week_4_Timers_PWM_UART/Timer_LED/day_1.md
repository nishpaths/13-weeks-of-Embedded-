Why is a hardware timer better than manually counting loop iterations?
- uses a known clock and runs independently of the main program 
What does the prescaler do?
- the prescaler tells the timer how frequently to increment (determines the ticks). 
What does the auto-reload value do?
- the auto reload value tells the timer when to update (at what interval you should update)

- PSC changes how much real time one counter increment represents. ARR determines how many of those increments occur before the event.

What is the difference between timer frequency and LED blink frequency?
What happened when the timer values were incorrect?
- check break notes


What happens if I break stuff notes:
Set the prescaler ten times too high 
- Makes it count ten times slower
Set the period ten times too low
- blinks way too fast, mabye ten times faster ?  
Forget to start the timer 
- LED just stays on
Use the wrong timer clock assumption
- Will toggle during different time/intervals