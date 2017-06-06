angular.module("ruServer").controller("adminManageCreditsCtrl", function ($scope, $http, $location, config) {

    $scope.students = [{
        name: "",
        registryNumber: "",
        cellphoneCredit: 0,
        cardCredit: 0,
        checked: false
    }];


    $scope.input = {
        phoneIncrement: 0,
        cardIncrement: 0
    };

    $http.get(config.serverBaseUrl + "loadStudents.php")
        .then(function (response) {

            $scope.students = response.data;

        });

    $scope.isChecked = function (students) {
        return students.some(function (student) {
            return student.checked;
        });
    };

    $scope.addCredits = function (input) {

        $scope.students.forEach(function (student) {
            if (student.checked) {
                var _token = localStorage['ruServer'];
                var _sourceAdmin = _token.slice(19);
                var _postData = {
                    sourceRegistryNumber: _sourceAdmin,
                    targetRegistryNumber: student.registryNumber,
                    diffCreditCellphone: input.phoneIncrement,
                    diffCreditTag: input.cardIncrement
                };
                $http.post(config.serverBaseUrl + "updateCredits.php", _postData)
                    .then(function (response) {
                        console.log(response.data);
                    })
                    .catch(function (error) {
                        console.log(error);
                    });
            }
        });
    };
});