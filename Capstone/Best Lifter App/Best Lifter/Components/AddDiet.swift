//
//  AddDiet.swift
//  Best Lifter
//
//  Created by νΌμμ on 2021/05/10.
//

import SwiftUI

struct AddDietByPhoto: View {
    @State private var showingImagePicker: Bool = false
    @State private var pickedImage: Image?
    @State private var sourceType: UIImagePickerController.SourceType = .photoLibrary
    @Environment(\.presentationMode) var presentationMode: Binding<PresentationMode>
    @ObservedObject var onDietViewModel: OnDietViewModel
    @ObservedObject var dietViewModel: DietViewModel
    
    var body: some View {
        VStack {
            Spacer()
            VStack {
                if let pickedImage = pickedImage {
                    pickedImage
                        .resizable()
                        .cornerRadius(10.0)
                        .aspectRatio(contentMode: .fit)
                        .overlay(
                            RoundedRectangle(cornerRadius: 10.0)
                                .stroke(lineWidth: 2)
                        )
                        .frame(width: 300, height: 300)
                } else {
                    Image("Food_Empty")
                        .renderingMode(.template)
                        .resizable()
                        .aspectRatio(contentMode: .fit)
                        .frame(width: 300, height: 300)
                        .foregroundColor(Color.white)
                        .background(Color.gray)
                        .cornerRadius(10.0)
                        .overlay(
                            RoundedRectangle(cornerRadius: 10.0)
                                .stroke(lineWidth: 2)
                        )
                }
                
                ForEach(onDietViewModel.predictedLables, id: \.self) { label in
                    Button(action: {
                        onDietViewModel.sendFoodNameToDBServer(foodName: label)
                        onDietViewModel.selectedFoodLabel = label
                        
                        dietViewModel.setNutritionCGFloat()
                        onDietViewModel.setNutritionToZero()
                        
                        presentationMode.wrappedValue.dismiss()
                        onDietViewModel.predictedLables.removeAll()
                    }, label: {
                        Text(label)
                            .font(.system(size: 20, weight: .regular, design: .default))
                            .frame(width: UIScreen.main.bounds.width - 80, height: 50)
                            .overlay(
                                RoundedRectangle(cornerRadius: 10.0)
                                    .stroke(lineWidth: 2)
                            )
                    })
                    .padding(.vertical, 5)
                    .foregroundColor(Color.black)
                        
                        
                }
            }
            
            Divider()
            
            VStack {
                HStack(spacing: 50) {
                    Button(action: {
                        self.sourceType = .camera
                        self.showingImagePicker.toggle()
                    }, label: {
                        Image("camera")
                            .renderingMode(.template)
                            .resizable()
                            .aspectRatio(contentMode: .fit)
                            .frame(width: 50, height: 50)
                            .foregroundColor(Color.black)
                    })
                    .fullScreenCover(isPresented: $showingImagePicker, content: {
                        ImagePicker(sourceType: $sourceType) { image in
                            self.pickedImage = Image(uiImage: image)
                            onDietViewModel.sendImageToPredictServer(image: image)
                        }
                        .ignoresSafeArea(.all)
                    })
                    
                    Button(action: {
                        self.sourceType = .photoLibrary
                        self.showingImagePicker.toggle()
                    }, label: {
                        Image("album")
                            .renderingMode(.template)
                            .resizable()
                            .aspectRatio(contentMode: .fit)
                            .frame(width: 50, height: 50)
                            .foregroundColor(Color.black)
                    })
                    .fullScreenCover(isPresented: $showingImagePicker, content: {
                        ImagePicker(sourceType: $sourceType) { image in
                            self.pickedImage = Image(uiImage: image)
                        }
                        .ignoresSafeArea(.all)
                    })
                }
                .padding(.bottom, 30)
                
                if pickedImage != nil {
                    Button(action: {
                        self.onDietViewModel.pickedImage = self.pickedImage
                        self.presentationMode.wrappedValue.dismiss()
                    }, label: {
                        Text("νμΈ")
                            .font(.system(size: 20, weight: .regular, design: .default))
                            .frame(width: UIScreen.main.bounds.width - 40, height: 30)
                            .overlay(
                                RoundedRectangle(cornerRadius: 10.0)
                                    .stroke(lineWidth: 2)
                            )
                            .foregroundColor(Color.black)
                    })
                    .padding(.bottom, 5)
                }
                
                Button(action: {
                    self.presentationMode.wrappedValue.dismiss()
                }, label: {
                    Text("μ·¨μ")
                        .font(.system(size: 20, weight: .regular, design: .default))
                        .frame(width: UIScreen.main.bounds.width - 40, height: 30)
                        .overlay(
                            RoundedRectangle(cornerRadius: 10.0)
                                .stroke(lineWidth: 2)
                        )
                        .foregroundColor(Color.black)
                })
            }
            .frame(maxHeight: .infinity, alignment: .bottom)
        }
    }
}

struct AddDietByUserInput: View {
    // BindingμΌλ‘ λ°κΈ°
    @State var foodString: String = String()
    @State var caloriesString: String = String()
    @State var carbString: String = String()
    @State var proteinString: String = String()
    @State var fatString: String = String()
    @State var isMealTimeSelectionView: Bool = false
    @Environment(\.presentationMode) var presentationMode: Binding<PresentationMode>
    @ObservedObject var onDietViewModel: OnDietViewModel
    @ObservedObject var dietViewModel: DietViewModel
    
    var body: some View {
        VStack(alignment: .center, spacing: 10) {
            Text("μμ μ λ³΄")
                .font(.title)
                .padding(.bottom, 10)
            Button(action: {
                self.isMealTimeSelectionView.toggle()
            }, label: {
                Text(onDietViewModel.mealTimeString)
                    .frame(width: UIScreen.main.bounds.width - 80, height: 30)
                    .foregroundColor(onDietViewModel.mealTimeString == "λΌλ" ? .gray : .black)
            })
            .overlay(
                RoundedRectangle(cornerRadius: 10)
                    .stroke(lineWidth: 2)
            )
            .fullScreenCover(isPresented: $isMealTimeSelectionView, content: {
                mealTimeSelectionView
            })
            
            TextField("μμλͺ", text: $foodString)
                .multilineTextAlignment(.center)
                .frame(width: UIScreen.main.bounds.width - 80, height: 30)
                .overlay(
                    RoundedRectangle(cornerRadius: 10)
                        .stroke(lineWidth: 2)
                )
            TextField("μΉΌλ‘λ¦¬", text: $caloriesString)
                .multilineTextAlignment(.center)
                .frame(width: UIScreen.main.bounds.width - 80, height: 30)
                .overlay(
                    RoundedRectangle(cornerRadius: 10)
                        .stroke(lineWidth: 2)
                )
                .keyboardType(.numberPad)
            TextField("νμνλ¬Ό", text: $carbString)
                .multilineTextAlignment(.center)
                .frame(width: UIScreen.main.bounds.width - 80, height: 30)
                .overlay(
                    RoundedRectangle(cornerRadius: 10)
                        .stroke(lineWidth: 2)
                )
                .keyboardType(.numberPad)
            TextField("λ¨λ°±μ§", text: $proteinString)
                .multilineTextAlignment(.center)
                .frame(width: UIScreen.main.bounds.width - 80, height: 30)
                .overlay(
                    RoundedRectangle(cornerRadius: 10)
                        .stroke(lineWidth: 2)
                )
                .keyboardType(.numberPad)
            TextField("μ§λ°©", text: $fatString)
                .multilineTextAlignment(.center)
                .frame(width: UIScreen.main.bounds.width - 80, height: 30)
                .overlay(
                    RoundedRectangle(cornerRadius: 10)
                        .stroke(lineWidth: 2)
                )
                .keyboardType(.numberPad)
            
            HStack {
                Button(action: {
                    self.presentationMode.wrappedValue.dismiss()
                    self.onDietViewModel.addFood(mealTime: onDietViewModel.mealTimeString, name: foodString, calorie: caloriesString, carb: carbString, protein: proteinString, fat: fatString)
                    self.onDietViewModel.mealTimeString = "λΌλ"
                    self.dietViewModel.userCalorieTaked += Int(caloriesString) ?? 0
                    self.dietViewModel.userCarbTaked += Int(carbString) ?? 0
                    self.dietViewModel.userProteinTaked += Int(proteinString) ?? 0
                    self.dietViewModel.userFatTaked += Int(fatString) ?? 0
                    self.dietViewModel.setNutritionCGFloat()
                    self.onDietViewModel.setNutritionToZero()
//                    print(Int(caloriesString))
//                    print("Add Diet User Calorire: \(dietViewModel.userCalorieTaked)")
                }, label: {
                    Text("νμΈ")
                        .frame(width: UIScreen.main.bounds.width / 2 - 45, height: 30)
                        .overlay(
                            RoundedRectangle(cornerRadius: 10)
                                .stroke(lineWidth: 2)
                        )
                        .foregroundColor(Color.black)
                })
                
                Button(action: {
                    self.presentationMode.wrappedValue.dismiss()
                    self.onDietViewModel.mealTimeString = "λΌλ"
                }, label: {
                    Text("μ·¨μ")
                        .frame(width: UIScreen.main.bounds.width / 2 - 45, height: 30)
                        .overlay(
                            RoundedRectangle(cornerRadius: 10)
                                .stroke(lineWidth: 2)
                        )
                        .foregroundColor(Color.black)
                })
            }
            .padding(.top, 20)
        }
        .padding()
        .overlay(
            RoundedRectangle(cornerRadius: 10)
                .stroke(lineWidth: 2)
        )
    }
    
    var mealTimeSelectionView: some View {
        VStack {
            Picker(selection: $onDietViewModel.mealTime, label: Text("λΌλ"), content: {
                Text("μμΉ¨").tag(1)
                Text("μ μ¬").tag(2)
                Text("μ λ").tag(3)
            })
            
            Button(action: {
                self.onDietViewModel.setMealTime(mealTime: onDietViewModel.mealTime)
                self.isMealTimeSelectionView.toggle()
            }, label: {
                Text("λ±λ‘")
                    .foregroundColor(Color.black)
            })
            .frame(width: 80, height: 30, alignment: .center)
            .overlay(
                RoundedRectangle(cornerRadius: 10.0)
                    .stroke(lineWidth: 2)
            )
            .padding(20)
        }
    }
}

struct AddDiet_Previews: PreviewProvider {
    static var previews: some View {
        AddDietByPhoto(onDietViewModel: OnDietViewModel(), dietViewModel: DietViewModel())
//        AddDietByUserInput()
    }
}
