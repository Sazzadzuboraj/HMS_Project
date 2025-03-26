#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Appointment
{
    int serialNumber;
    string patientName;
    string doctorName;
    string date; // format: yyyy-mm-dd
    string time; // format: hh:mm
};

struct Test
{
    string testName;
    string timeRange;
    int price;
};

// Base class for inheritance and polymorphism
class HospitalEntity
{
public:
    virtual void displayDetails() const = 0; // Pure virtual function
    virtual ~HospitalEntity() {}            // Virtual destructor
};

// Derived class for Patients
class Patient : public HospitalEntity
{
    string name;
    int age;

public:
    Patient(const string &name, int age) : name(name), age(age) {}

    void displayDetails() const override
    {
        cout << "Patient Name: " << name << ", Age: " << age << "\n";
    }
};

// Derived class for Doctors
class Doctor : public HospitalEntity
{
    string name;
    string specialization;

public:
    Doctor(const string &name, const string &specialization) : name(name), specialization(specialization) {}

    void displayDetails() const override
    {
        cout << "Doctor Name: " << name << ", Specialization: " << specialization << "\n";
    }
};

class HospitalManagement
{
private:
    vector<Appointment> appointments;
    vector<Test> tests;
    vector<HospitalEntity *> hospitalEntities; // Polymorphic entities

public:
    HospitalManagement()
    {
        initializeTests();
    }

    ~HospitalManagement()
    {
        for (auto entity : hospitalEntities)
        {
            delete entity; // Clean up dynamically allocated entities
        }
    }

    void initializeTests()
    {
        tests.push_back({"X-ray", "8am - 8pm", 300});
        tests.push_back({"MRI", "8pm - 10pm", 4200});
        tests.push_back({"Blood Film", "8am -10pm", 300});
        tests.push_back({"SGPT", "8am -10pm", 350});
        tests.push_back({"Anti HAV", "8am -10pm", 1200});
        tests.push_back({"Anti HBc", "8am -10pm", 1500});
    }

    void addHospitalEntity(HospitalEntity *entity)
    {
        hospitalEntities.push_back(entity);
    }

    void displayHospitalEntities() const
    {
        for (const auto &entity : hospitalEntities)
        {
            entity->displayDetails();
        }
    }

    void registerPatient(const string &patientName, int patientAge, const string &doctorName, const string &currentDate, const string &appointmentDate, const string &time)
    {
        if (!isNextDay(currentDate, appointmentDate))
        {
            cout << "Appointments can only be scheduled for the next day.\n";
            return;
        }

        for (const auto &appointment : appointments)
        {
            if (appointment.date == appointmentDate && appointment.time == time && appointment.doctorName == doctorName)
            {
                cout << "This doctor already has an appointment at the same date and time. Please choose another slot.\n";
                return;
            }

            if (appointment.date == appointmentDate && isOverlapping(appointment.time, time))
            {
                cout << "This time slot is already booked. Please choose another time.\n";
                return;
            }
        }

        int serialNumber = appointments.size() + 1; // Increment serial number based on the total appointments
        appointments.push_back({serialNumber, patientName, doctorName, appointmentDate, time});

        // Create a new Patient object and add it to the hospitalEntities
        addHospitalEntity(new Patient(patientName, patientAge));
        cout << "Appointment registered successfully for " << patientName << " (Age: " << patientAge << ") with Serial Number: " << serialNumber << ".\n";
    }

    void cancelAppointment(int serialNumber)
    {
        for (auto it = appointments.begin(); it != appointments.end(); ++it)
        {
            if (it->serialNumber == serialNumber)
            {
                appointments.erase(it);
                cout << "Appointment with Serial Number " << serialNumber << " canceled successfully.\n";
                return;
            }
        }
        cout << "No matching appointment found.\n";
    }

    void displayAppointmentsByPatient(const string &patientName)
    {
        bool found = false;
        cout << "Serial No.   Patient Name             Doctor Name              Date            Time\n";
        cout << string(85, '-') << "\n";
        for (const auto &appointment : appointments)
        {
            if (appointment.patientName == patientName)
            {
                cout << appointment.serialNumber << "            " << appointment.patientName << "              " << appointment.doctorName << "             " << appointment.date << "              " << appointment.time << "\n";
                found = true;
            }
        }
        if (!found)
        {
            cout << "No appointments found for patient: " << patientName << "\n";
        }
    }

    void displayTestDetails()
    {
        cout << "Serial No.  Test Name       Time Range          Price (Tk)\n";
        cout << string(60, '-') << "\n";
        for (size_t i = 0; i < tests.size(); ++i)
        {
            cout << i + 1 << "          " << tests[i].testName << "          " << tests[i].timeRange << "            " << tests[i].price << "\n";
        }
    }

    void calculateTestCost(const vector<int> &selectedTestIndices)
    {
        int totalCost = 0;
        for (const auto &index : selectedTestIndices)
        {
            if (index >= 1 && index <= tests.size())
            {
                totalCost += tests[index - 1].price; // Adjust for 0-based index
            }
            else
            {
                cout << "Test index " << index << " not found.\n";
            }
        }
        cout << "Total test cost: " << totalCost << " Tk\n";
    }

    bool isNextDay(const string &currentDate, const string &appointmentDate)
    {
        int curYear, curMonth, curDay;
        int appYear, appMonth, appDay;
        sscanf(currentDate.c_str(), "%d-%d-%d", &curYear, &curMonth, &curDay);
        sscanf(appointmentDate.c_str(), "%d-%d-%d", &appYear, &appMonth, &appDay);

        return appYear == curYear && appMonth == curMonth && appDay == curDay + 1;
    }

    bool isOverlapping(const string &existingTime, const string &newTime)
    {
        int existingHour, existingMinute;
        int newHour, newMinute;
        sscanf(existingTime.c_str(), "%d:%d", &existingHour, &existingMinute);
        sscanf(newTime.c_str(), "%d:%d", &newHour, &newMinute);

        int existingStart = existingHour * 60 + existingMinute;
        int existingEnd = existingStart + 20; // 20-minute slot
        int newStart = newHour * 60 + newMinute;
        int newEnd = newStart + 20; // 20-minute slot

        return (newStart < existingEnd && newEnd > existingStart);
    }
};

int main()
{
    HospitalManagement hospital;
    int choice;

    // Add some initial hospital entities
    hospital.addHospitalEntity(new Doctor("Dr. Ikbal Hossain", "Cardiology"));
    hospital.addHospitalEntity(new Doctor("Dr. Kamruzzaman", "Neurology"));
    hospital.addHospitalEntity(new Doctor("Dr. Debasish Pal", "Orthopedics"));

    do
    {
        cout << "\nHospital Management System\n";
        cout << "1. Display Hospital Entities\n";
        cout << "2. Register Patient\n";
        cout << "3. Display Appointments by Patient Name\n";
        cout << "4. Cancel Appointment\n";
        cout << "5. Display Test Details\n";
        cout << "6. Calculate Test Cost\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "\nHospital Entities:\n";
            hospital.displayHospitalEntities();
            break;
        case 2:
        {
            string patientName, doctorName, currentDate, appointmentDate, time;
            int patientAge;
            cout << "Enter patient name: ";
            cin.ignore();
            getline(cin, patientName);
            cout << "Enter patient age: ";
            cin >> patientAge;
            cout << "Enter doctor name: ";
            cin.ignore();
            getline(cin, doctorName);
            cout << "Enter current date (yyyy-mm-dd): ";
            cin >> currentDate;
            cout << "Enter appointment date (yyyy-mm-dd): ";
            cin >> appointmentDate;
            cout << "Enter appointment time (hh:mm): ";
            cin >> time;
            hospital.registerPatient(patientName, patientAge, doctorName, currentDate, appointmentDate, time);
            break;
        }
        case 3:
        {
            string patientName;
            cout << "Enter patient name to search: ";
            cin.ignore();
            getline(cin, patientName);
            hospital.displayAppointmentsByPatient(patientName);
            break;
        }
        case 4:
        {
            int serialNumber;
            cout << "Enter appointment serial number to cancel: ";
            cin >> serialNumber;
            hospital.cancelAppointment(serialNumber);
            break;
        }
        case 5:
            hospital.displayTestDetails();
            break;
        case 6:
        {
            int numTests;
            cout << "Enter number of tests (by serial number): ";
            cin >> numTests;
            vector<int> selectedTestIndices(numTests);
            cout << "Enter test serial numbers:\n";
            for (int i = 0; i < numTests; ++i)
            {
                cin >> selectedTestIndices[i];
            }
            hospital.calculateTestCost(selectedTestIndices);
            break;
        }

        case 7:
            cout << "Exiting system. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
    while (choice != 7);

    return 0;
}
