#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <limits>
#include <unordered_set>

using namespace std;

class Course {
public:
    string name;
    int credits;
    string schedule;
    int courseNumber;
    int minCredits;
    int maxCredits;

    Course() : name(""), credits(0), schedule(""), courseNumber(0), minCredits(1), maxCredits(4) {}

    void print() const {
        cout << courseNumber << ". Course Name: " << name << "  |  ";
        cout << "Credits: " << minCredits << "-" << maxCredits << "  |  ";
        cout << "Course Timings: " << schedule << endl;
    }
};

class CourseBuilder {
private:
    Course currentCourse;
    vector<Course> courseList;

public:
    CourseBuilder& setName(const string& name) {
        currentCourse.name = name;
        return *this;
    }

    CourseBuilder& setCreditsRange(int min, int max) {
        if (min < 0 || max < min) {
            throw invalid_argument("Invalid credit range");
        }
        currentCourse.minCredits = min;
        currentCourse.maxCredits = max;
        return *this;
    }
    
    CourseBuilder& setSchedule(const string& schedule) {
        currentCourse.schedule = schedule;
        return *this;
    }

    void addCourse() {
        currentCourse.courseNumber = static_cast<int>(courseList.size());
        courseList.push_back(currentCourse);
        currentCourse = Course(); 
    }

    void printCurrentCourse() const {
        currentCourse.print();
    }

    void showAllCourses() const {
        for (const auto& course : courseList) {
            course.print();
        }
    }

    const vector<Course>& getCourseList() const {
        return courseList;
    }

    Course& getLastAddedCourse() {
        if (courseList.empty()) {
            throw runtime_error("No courses added yet");
        }
        return courseList.back();
    }
};

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getValidInput(int min, int max) {
    int input;
    while (true) {
        if (cin >> input && input >= min && input <= max) {
            clearInputBuffer();
            return input;
        } else {
            cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
            clearInputBuffer();
        }
    }
}

int main() {
    cout << "List of available courses is as follows:\n" << endl;
    
    CourseBuilder availableCourses;
    availableCourses.setName("CS330").setCreditsRange(10, 14).setSchedule("10AM - 11AM").addCourse();
    availableCourses.setName("CS340").setCreditsRange(7, 9).setSchedule("11AM - 12PM").addCourse();
    availableCourses.setName("CS345").setCreditsRange(9, 13).setSchedule("8AM - 9AM").addCourse();
    availableCourses.setName("CS455").setCreditsRange(7, 10).setSchedule("2PM - 3PM").addCourse();
    availableCourses.setName("CS771").setCreditsRange(9, 12).setSchedule("12PM - 1PM").addCourse();
    availableCourses.setName("CS253").setCreditsRange(10, 13).setSchedule("12PM - 1PM").addCourse();

    availableCourses.showAllCourses();

    cout << "\nChoose the courses and add the number of credits for each course\n";

    CourseBuilder studentSchedule;
    const int coursesToRegister = 2;
    unordered_set<int> selectedCourses;

    for (int i = 0; i < coursesToRegister; i++) {
        cout << "Choose your " << (i + 1) << "th course\n";
        int courseNumber;
        bool validSelection = false;

        while (!validSelection) {
            cout << "Enter the number of the course you want to pick (0-" << availableCourses.getCourseList().size() - 1 << "): ";
            courseNumber = getValidInput(0, static_cast<int>(availableCourses.getCourseList().size()) - 1);

            if (selectedCourses.find(courseNumber) != selectedCourses.end()) {
                cout << "Error: You have already selected this course. Please choose a different one.\n";
            } else {
                validSelection = true;
            }
        }

        selectedCourses.insert(courseNumber);
        const Course& selectedCourse = availableCourses.getCourseList()[courseNumber];
        
        cout << "Enter number of credits (" << selectedCourse.minCredits << "-" << selectedCourse.maxCredits << "): ";
        int credits = getValidInput(selectedCourse.minCredits, selectedCourse.maxCredits);

        studentSchedule.setName(selectedCourse.name)
                       .setCreditsRange(selectedCourse.minCredits, selectedCourse.maxCredits)
                       .setSchedule(selectedCourse.schedule)
                       .addCourse();
        
        // Set the chosen credits for the student's course
        studentSchedule.getLastAddedCourse().credits = credits;
    }

    cout << "\nYour semester timetable will look like this:" << endl;
    for (const auto& course : studentSchedule.getCourseList()) {
        cout << course.courseNumber << ". Course Name: " << course.name << "  |  ";
        cout << "Credits: " << course.credits << "  |  ";
        cout << "Course Timings: " << course.schedule << endl;
    }

    return 0;
}