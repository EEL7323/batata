angular.module("ruServer").controller("adminManageUsersCtrl", function ($scope, $http, $location, config, $rootScope, authenticationService) {

    // Variables declaration or attribution

    // Variables of the error alert
    $scope.showAlert = false;
    $scope.isError = false;
    $scope.errorText = "";

    // Variables of the filter
    $scope.orderCriteria = 'name';
    $scope.orderDirection = false;

    // Initialization of variable users, used to feed the table showed in the screen
    $scope.users = [{
        name: "",
        registryNumber: "",
        tagNumber: 0,
        cellPhoneCredit: 0,
        cardCredit: 0,
        accessLevel: 1,
        checked: false
    }];

    // Variable that stores the value of the token
    var _token = localStorage['ruServer'];

    // Functions declaration

    // Utilization of the authentication service to check if the user is allowed to be in this page, if not load the login page
    var _checkAuthenticationForPage = function () {
        if (typeof (_token) != "undefined") {
            authenticationService.checkToken(_token)
                .then(function (response) {
                    var _allowedAccess = response.access;
                    var _accessLevel = response.accessLevel;
                    if (!_allowedAccess || _accessLevel != 0) $location.path("/login");
                })
                .catch(function (error) {
                    $rootScope.phpError = error.data;
                    $location.path("/error");
                });
        }
        else $location.path("/login");
    };

    // Function to load basic information of the users
    var _loadUsers = function () {
		var _postData = {
			registryNumber: "all"
        };
        // Post request to get data from loadUsers.php, with the _postData indicating that it should load data from all users
        $http.post(config.serverBaseUrl + "loadUsers.php", _postData)
            .then(function (response) {
                $scope.users = response.data;
                if (typeof ($scope.users) == "string") {
                    $scope.showAlert = true;
                    $scope.isError = true;
                    $scope.errorText = $scope.users;
                    $scope.users = [];
                }
            }) // Error handling
            .catch(function (error) {
                $rootScope.phpError = error.data;
                $location.path("/error");
            });
    };

    // Function to rotate an array
    var _rotateArray = function (array) {
        var _size = array.length;
        var _newArray = [];
        for (i = 0; i < _size; i++) {
            _newArray[i] = array[_size - i - 1];
        }
        return _newArray;
    };

    // Function to order data showed in the table
    $scope.orderBy = function (field) {
        $scope.orderCriteria = field;
        $scope.orderDirection = !$scope.orderDirection;

        switch (field) {
            case 'name':
                if ($scope.orderDirection) $scope.nameClass = "glyphicon glyphicon-triangle-top";
                else $scope.nameClass = "glyphicon glyphicon-triangle-bottom";
                $scope.registryNumberClass = "";
                $scope.tagNumberClass = "";
                break;
            case 'registryNumber':
                if ($scope.orderDirection) $scope.registryNumberClass = "glyphicon glyphicon-triangle-top";
                else $scope.registryNumberClass = "glyphicon glyphicon-triangle-bottom";
                $scope.nameClass = "";
                $scope.tagNumberClass = "";
                break;
            case 'tagNumber':
                if ($scope.orderDirection) $scope.tagNumberClass = "glyphicon glyphicon-triangle-top";
                else $scope.tagNumberClass = "glyphicon glyphicon-triangle-bottom";
                $scope.nameClass = "";
                $scope.registryNumberClass = "";
                break;
            default:
                $scope.nameClass = "";
                $scope.registryNumber = "";
                $scope.tagNumberClass = "";
        }
    };

    // Function to change the color of the warning box, depending if there is an error or not
    $scope.getColor = function () {
        if ($scope.isError == true) return "alert-danger";
        else return "alert-success";
    };

    // Count the number of rows where the checkbox is checked
    $scope.countChecked = function (users) {
        _acum = 0;
        if (typeof(users) == "object") {
            users.forEach(function (user) {
                if (user.checked) _acum++;
            });
        }
        return _acum;
    };

    // Function to toggle/untoggle all checkboxes
    $scope.toggleAll = function (toggle, users) {
        if (typeof(users) == "object") {
            users = users.filter(function (user) {
                var _searchCriteriaUpperCase = typeof ($scope.searchCriteria) == "undefined" ? "" : $scope.searchCriteria.toUpperCase();
                if (user.name.toUpperCase().indexOf(_searchCriteriaUpperCase) != -1 || user.registryNumber.toUpperCase().indexOf(_searchCriteriaUpperCase) != -1 || user.tagNumber.toUpperCase().indexOf(_searchCriteriaUpperCase) != -1) return user;
            });

            if (toggle) {
                users.forEach(function (user) {
                    user.checked = true;
                });
            }
            else {
                users.forEach(function (user) {
                    user.checked = false;
                });
            }
        }
    };

    // Function to go to the form to insert a new user
    $scope.insertUser = function () {
        $location.path("/admin/manage-users/edit-user");
        $rootScope.selUser = {
            name: "",
            registryNumber: "",
            tagNumber: "",
            accessLevel: ""
        };
        $rootScope.editUserMethod = "Insert user info";
    };

    // Function to go to the pre filled form to edit an user
    $scope.editUser = function (users) {
        $location.path("/admin/manage-users/edit-user");
        users.forEach(function (user) {
            if (user.checked) {
                $rootScope.selUser = {
                    name: user.name,
                    registryNumber: user.registryNumber,
                    tagNumber: (user.tagNumber * 1),
                    accessLevel: user.accessLevel
                };
                $rootScope.editUserMethod = "Edit user info";
            }
        });
    };

    // Function to delete an user in the database and update the list
    $scope.deleteUsers = function (users) {
        var _accessLevel = _token.slice(7, 8);
        var _sourceAdmin = _token.slice(19);
        var _postData = {
            usersToDelete: [],
            sourceAdmin: _sourceAdmin
        };
        var _indexesToDelete = [];
        users.forEach(function (user) {
            if (user.checked) {
                _postData.usersToDelete.push(user.registryNumber);
                _indexesToDelete.push(users.indexOf(user));
            }
        });
        // Post request to delete selected users stored in the array usersToDelete
        $http.post(config.serverBaseUrl + "deleteUsers.php", _postData)
            .then(function (response) {
                $scope.showAlert = true;
                $scope.errorText = response.data;
                if (response.data.startsWith("Error -")) $scope.isError = true;
                else {
                    $scope.isError = false;
                    _indexesToDelete = _rotateArray(_indexesToDelete);
                    _indexesToDelete.forEach(function (index) {
                        $scope.users.splice(index, 1);
                    });
                }
            })
            .catch(function (error) { // Error handling
                $rootScope.phpError = error.data;
                $location.path("/error");
            });

    };

    // Initialization

    _checkAuthenticationForPage();
    _loadUsers();

});