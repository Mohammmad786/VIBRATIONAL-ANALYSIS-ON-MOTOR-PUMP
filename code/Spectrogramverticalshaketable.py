% Load displacement-time data from an Excel sheet
displacement_data = xlsread('Vertical_DVST.xlsx'); % Replace with your file name

% Extract time and displacement from the loaded data
time = displacement_data(:, 1);
displacement = displacement_data(:, 2);

% Plot original displacement
figure;
subplot(3, 1, 1);
plot(time, displacement, 'DisplayName', 'Displacement');
title('Displacement');
xlabel('Time');
ylabel('Displacement');
legend('show'); % Display the legend for this subplot

% Adjust y-axis limits for better visualization
ylim('auto');

% Numerically integrate displacement to get velocity
velocity = cumtrapz(time, displacement);

% Numerically integrate velocity to get acceleration
acceleration = cumtrapz(time, velocity);

% Subplot 2: Velocity
subplot(3, 1, 2);
plot(time, velocity, 'DisplayName', 'Velocity');
title('Velocity');
xlabel('Time');
ylabel('Velocity');
legend('show'); % Display the legend for this subplot

% Subplot 3: Acceleration
subplot(3, 1, 3);
plot(time, acceleration, 'DisplayName', 'Acceleration');
title('Acceleration');
xlabel('Time');
ylabel('Acceleration');
legend('show', 'Location', 'Best'); % Display the legend for this subplot, specify the 'Best' location

% Adjust y-axis limits for better visualization
ylim('auto');

% STFT parameters
window_size = 256;  % Size of the window for STFT
overlap = 200;     % Overlap between adjacent windows

% Compute and plot the STFT for displacement
figure;
spectrogram(displacement, window_size, overlap, [], 'yaxis');
title('STFT of Displacement');
xlabel('Time');
ylabel('Frequency');

% Optionally, you can use additional subplots to visualize the STFTs for velocity and acceleration
% Assuming you have already loaded displacement data and performed the STFT
% ...

% Extract the STFT data for creating a table
time_vector = repelem(t, length(f));
frequency_vector = repmat(f, 1, length(t)).';
amplitude_vector = abs(s(:));

% Create a table
spectrogram_table = table(time_vector, frequency_vector, amplitude_vector, 'VariableNames', {'Time', 'Frequency', 'Amplitude'});

% Save only time vs. frequency data to a CSV file
time_frequency_table = spectrogram_table(:, {'Time', 'Frequency'});
writetable(time_frequency_table, 'time_vs_frequency_data.csv');

disp('CSV file "time_vs_frequency_data.csv" created successfully.');